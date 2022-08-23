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

#include "atomsciflow/abinit/utils.h"
#include "atomsciflow/server/submit_script.h"
#include "atomsciflow/remote/server.h"

namespace atomsciflow {

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
    this->set_pot("ncpp");

    this->set_param("ecut", 15);
    this->set_param("occopt", 3);
    this->set_param("nstep", 100);
    this->set_param("diemac", 2.0);
    this->set_param("ixc", 11);
    this->use_tol("tolvrs", 1.0e-18, 0);
    
    job.set_run("cmd", "$ASF_CMD_ABINIT");
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

    this->set_param("pp_dirpath[0]", "\"./\"");
    std::ostringstream pseudos;
    pseudos << "\"";
    for (int i = 0; i < this->datasets[0]->system->elem_typat.size(); i++) {
        for (const auto& item : this->datasets[0]->system->elem_typat) {
            if (item.second == (i+1)) {
                if (i == 0) {
                    pseudos << item.first << this->pseudo_ext;
                } else {
                    pseudos << ", " << item.first << this->pseudo_ext;
                }
                break; 
            }
        }
    }
    pseudos << "\"";
    this->set_param("pseudos[0]", pseudos.str());
}

/**
 * Usage: 
 *      this->set_param("ecut[0]", 15)
 *      this->set_param("ecut[1]", 15)
 *  if no [x] specified, will set dataset 0 by default.
 *  i.e. 
 *      this->set_param("ecut", 15) equals to
 *      this->set_param("ecut[0]", 15)
 * dataset_i:
 * if the specified dataset_i is larger than this->ndtset, the program
 * will be stopped.
 * 
 */ 
template <typename T>
void Abinit::set_param(std::string key, T value) {
    std::vector<std::string> str_vec_1;
    std::vector<std::string> str_vec_2;
    boost::split(str_vec_1, key, boost::is_any_of("["));
    if (1 == str_vec_1.size()) {
        this->datasets[0]->set_param(key, value);
    } else {
        boost::split(str_vec_2, str_vec_1[1], boost::is_any_of("]"));
        int dataset_i = boost::lexical_cast<int>(str_vec_2[0]);
        if (dataset_i > this->ndtset) {
            std::cout << "------------------------------------------------------------------------" << "\n";
            std::cout << "Warning: atomsciflow::Abinit::set_param\n";
            std::cout << "specified ndtset larger than this->ndtset\n";
            std::cout << "------------------------------------------------------------------------" << "\n";
            std::exit(1);
        }
        this->datasets[dataset_i]->set_param(str_vec_1[0], value);
    }
}

void Abinit::py_set_param(std::string key, int value) {
    this->set_param(key, value);
}

void Abinit::py_set_param(std::string key, double value) {
    this->set_param(key, value);
}

void Abinit::py_set_param(std::string key, std::string value) {
    this->set_param(key, value);
}

void Abinit::py_set_param(std::string key, std::vector<int> value) {
    this->set_param(key, value);
}

void Abinit::py_set_param(std::string key, std::vector<double> value) {
    this->set_param(key, value);
}

void Abinit::py_set_param(std::string key, std::vector<std::string> value) {
    this->set_param(key, value);
}

void Abinit::py_set_param(std::string key, std::vector<std::vector<int>> value) {
    this->set_param(key, value);
}

void Abinit::py_set_param(std::string key, std::vector<std::vector<double>> value) {
    this->set_param(key, value);
}

void Abinit::py_set_param(std::string key, std::vector<std::vector<std::string>> value) {
    this->set_param(key, value);
}

/**
 * @brief Abinit::set_params set the params of dataset_i-th AbinitInput
 * @param params the Abinit input params
 */
template <typename T>
void Abinit::set_params(std::map<std::string, T>& params) {
    for (auto& item : params) {
        this->set_param(item.first, item.second);
    }
}

void Abinit::py_set_params(std::map<std::string, int>& params) {
    this->set_params(params);
}

void Abinit::py_set_params(std::map<std::string, double>& params) {
    this->set_params(params);
}

void Abinit::py_set_params(std::map<std::string, std::string>& params) {
    this->set_params(params);
}

void Abinit::py_set_params(std::map<std::string, std::vector<int>>& params) {
    this->set_params(params);
}

void Abinit::py_set_params(std::map<std::string, std::vector<double>>& params) {
    this->set_params(params);
}

void Abinit::py_set_params(std::map<std::string, std::vector<std::string>>& params) {
    this->set_params(params);
}

void Abinit::py_set_params(std::map<std::string, std::vector<std::vector<int>>>& params) {
    this->set_params(params);
}

void Abinit::py_set_params(std::map<std::string, std::vector<std::vector<double>>>& params) {
    this->set_params(params);
}

void Abinit::py_set_params(std::map<std::string, std::vector<std::vector<std::string>>>& params) {
    this->set_params(params);
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

void Abinit::set_pot(const std::string& pot) {
    if (boost::to_lower_copy(pot) == "ncpp") {
        this->pseudo_ext = ".psp8";
    } else if (boost::to_lower_copy(pot) == "paw") {
        this->pseudo_ext = ".GGA_PBE-JTH.xml";
    } else {
        this->pseudo_ext = ".psp8";
    }
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
        auto abinit_input = new AbinitInput{};
        this->datasets.push_back(abinit_input);
        abinit_input->n = this->datasets.size() - 1;
    }
}

void Abinit::run(const std::string& directory) {
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << "cat >abinit.abi<<EOF\n";
    step << this->to_string();
    step << "EOF\n";
    step << "cp";
    for (const auto& item : this->datasets[0]->system->xyz.elements_set) {
        if (this->pseudo_ext == ".psp8") {
            step << " " << (fs::path(config.get_pseudo_pot_dir()["abinit"]) / "pbe_s_sr/" / (item + ".psp8")).string();
        } else if (this->pseudo_ext == ".GGA_PBE-JTH.xml") {
            step << " " << (fs::path(config.get_pseudo_pot_dir()["abinit"]) / "JTH-PBE-atomicdata-1.1/ATOMICDATA/" / (item + ".GGA_PBE-JTH.xml")).string();
        } else {
            step << " " << (fs::path(config.get_pseudo_pot_dir()["abinit"]) / "pbe_s_sr/" / (item + ".psp8")).string();
        }
    }
    step << " ./\n";
    step << boost::format("$CMD_HEAD %1% abinit.abi\n") % this->job.run_params["cmd"];
    job.steps.push_back(step.str());
    step.clear();

    job.run(directory);
}

// explicit template instantiation
template void Abinit::set_param<int>(std::string, int);
template void Abinit::set_param<double>(std::string, double);
template void Abinit::set_param<std::string>(std::string, std::string);
template void Abinit::set_param<std::vector<int>>(std::string, std::vector<int>);
template void Abinit::set_param<std::vector<double>>(std::string, std::vector<double>);
template void Abinit::set_param<std::vector<std::string>>(std::string, std::vector<std::string>);
template void Abinit::set_param<std::vector<std::vector<int>>>(std::string, std::vector<std::vector<int>>);
template void Abinit::set_param<std::vector<std::vector<double>>>(std::string, std::vector<std::vector<double>>);
template void Abinit::set_param<std::vector<std::vector<std::string>>>(std::string, std::vector<std::vector<std::string>>);

template void Abinit::set_params<int>(std::map<std::string, int>&);
template void Abinit::set_params<double>(std::map<std::string, double>&);
template void Abinit::set_params<std::string>(std::map<std::string, std::string>&);
template void Abinit::set_params<std::vector<int>>(std::map<std::string, std::vector<int>>&);
template void Abinit::set_params<std::vector<double>>(std::map<std::string, std::vector<double>>&);
template void Abinit::set_params<std::vector<std::string>>(std::map<std::string, std::vector<std::string>>&);
template void Abinit::set_params<std::vector<std::vector<int>>>(std::map<std::string, std::vector<std::vector<int>>>&);
template void Abinit::set_params<std::vector<std::vector<double>>>(std::map<std::string, std::vector<std::vector<double>>>&);
template void Abinit::set_params<std::vector<std::vector<std::string>>>(std::map<std::string, std::vector<std::vector<std::string>>>&);

} // namespace atomsciflow
