/************************************************************************
MIT License

Copyright (c) 2021 Deqi Tang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
************************************************************************/

/// @file src/atomsciflow/abinit/abinit.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Tue 08 Mar 2022 02:35:36 PM CST

#include "atomsciflow/abinit/abinit.h"

#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include "atomsciflow/abinit/utils.h"
#include "atomsciflow/server/submit_script.h"
#include "atomsciflow/remote/server.h"

namespace atomsciflow {

namespace fs = boost::filesystem;

/**
 * @brief Abinit::Abinit
 * The constructor of class Abinit. It's responsible for the
 * initialization of the containers for AbinitInput, the
 * pseudo potential input string and the JobScheduler.
 */
Abinit::Abinit() {
    this->ndtset = 1;
    int i = 0;
    while (i < this->ndtset + 1) {
        this->datasets.emplace_back(new AbinitInput{});
        this->datasets.back()->set_n(i);
        i++;
    }
    this->datasets[0]->electrons->basic_setting();
    this->pseudo_input_str = "";
    job.set_run("cmd", "$ABINIT_BIN");
    job.set_run("script_name_head", "abinit-run");
}

/**
 * @brief Abinit::~Abinit
 * In the future, the raw pointer to AbinitInput might
 * be replaced with smart pointers.
 */
Abinit::~Abinit() {
    for (auto dt : this->datasets) {
        delete dt;
    }
}

/**
 * @brief Abinit::get_xyz get the structure file information
 * @param xyzfile the path to the xyz structure file
 * The source xyz structure file is used to set the system
 * of dataset 0 only.
 */
void Abinit::get_xyz(const std::string& xyzfile) {
    this->datasets[0]->system->get_xyz(xyzfile);
    job.set_run("xyz_file", fs::absolute(xyzfile).string());
    this->set_job_steps_default();
}

/**
 * @brief Abinit::set_params set the params of dataset_i-th AbinitInput
 * @param params the Abinit input params
 * @param dataset_i
 * if the specified dataset_i is larger than this->ndtset, the program
 * will be stopped.
 */
void Abinit::set_params(std::map<std::string, std::string>& params, int dataset_i = 0) {
    if (dataset_i > this->ndtset) {
        std::cout << "------------------------------------------------------------------------" << "\n";
        std::cout << "Warning: atomsciflow::Abinit::set_params\n";
        std::cout << "specified ndtset larger than this->ndtset\n";
        std::cout << "------------------------------------------------------------------------" << "\n";
        std::exit(1);
    }
    this->datasets[dataset_i]->set_params(params);
}

void Abinit::set_kpoints(std::map<std::string, std::string>& kpoints, int ndtset=0) {
    if (ndtset > this->ndtset) {
        std::cout << "------------------------------------------------------------------------" << "\n";
        std::cout << "Warning: atomsciflow::Abinit::set_params\n";
        std::cout << "specified ndtset larger than this->ndtset\n";
        std::cout << "------------------------------------------------------------------------" << "\n";
        std::exit(1);
    }
    this->datasets[ndtset]->set_kpoints(kpoints);
}

void Abinit::set_pseudos(const std::string& directory) {
    std::ostringstream out;
    out << "pp_dirpath \"" << directory << "\"\n";
    out << "pseudos \"";
    for (const auto& element : this->datasets[0]->system->xyz.elements_set) {
        out << " " << element << "," << element << ".psp8";
        //out << " " << element << "," << element << ".GGA_PBE-JTH.xml";
    }
    // remove the last unwanted extra comma
    this->pseudo_input_str = out.str();
    this->pseudo_input_str = this->pseudo_input_str.substr(this->pseudo_input_str.size() - 1);
    this->pseudo_input_str += "\"\n";
}

/// \brief Abinit::to_string
/// \return the Abinit input as a string
/// The AbinitSystem related setting for dataset other than 0 is
/// prohibited.
std::string Abinit::to_string() {
    std::string out = "";
    out = "ndtset ";
    out += std::to_string(this->ndtset);
    out += "\n";
    out += dataset_n_title(0);
    out += "\n";
    out += this->datasets[0]->to_string();
    for (int i = 1; i < this->ndtset + 1; i++) {
        this->datasets[i]->system->status = false;
        out += dataset_n_title(i);
        out += "\n";
        out += this->datasets[i]->to_string();
    }

    out += this->pseudo_input_str;

    return out;
}

/**
 * @brief Abinit::set_ndtset set the value of this->ndtset
 * and make adjustments to the AbinitInput container this->datasets
 * Thus, never modify the value of this->ndtset directly. Using
 * Abinit::set_ndtset should always be the only option.
 * @param ndtset int
 */
void Abinit::set_ndtset(int ndtset = 0) {
    this->ndtset = ndtset;
    while (this->datasets.size() - 1 < ndtset) {
        this->datasets.push_back(new AbinitInput());
        this->datasets[-1]->n = this->datasets.size() - 1;
    }
}

void Abinit::set_job_steps_default() {
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << "cat > " << this->files.main_in << "<<EOF\n";
    step << this->to_string();
    step << "EOF\n";
    step << "cat > " << this->files.name << "<<EOF\n";
    step << this->files.to_string(*this->datasets[0]->system);
    step << "EOF\n";
    step << "$CMD_HEAD " << this->job.run_params["cmd"] << " < " << this->files.name << "\n";
    job.steps.push_back(step.str());
    step.clear();
}

void Abinit::run(const std::string& directory) {
    job.run(directory);
}

} // namespace atomsciflow
