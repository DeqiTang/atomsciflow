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

/// @file src/atomsciflow/orca/orca.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 03:37:10 PM CST

#include "atomsciflow/orca/orca.h"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <iomanip>

#include "atomsciflow/server/submit_script.h"
#include "atomsciflow/remote/server.h"
#include "atomsciflow/base/element.h"

namespace atomsciflow {

namespace fs = boost::filesystem;

Orca::Orca() {

    keywords.push_back("HF");
    //keywords.push_back("def2-TZVP");
    keywords.push_back("6-31+G(d,p)");
    keywords.push_back("AUTOAUX");

    job.set_run_default("llhpc");
    job.set_run_default("pbs");
    job.set_run_default("bash");
    job.set_run_default("lsf_sz");
    job.set_run_default("lsf_sustc");
    job.set_run_default("cdcloud");

    job.set_run("cmd", "$ORCA_BIN");
    job.set_run("script_name_head", "orca-run");
}

Orca::~Orca() {

}

std::string Orca::to_string() {
    std::string out = "";

    out += "!";
    for (const auto& item : this->keywords) {
        out += " ";
        out += item;
    }
    out += "\n";
    out += "\n";
    for (auto& item : this->blocks) {
        out += item.second->to_string();
        out += "\n";
    }

    return out;
}

void Orca::new_block(const std::string& name) {
    this->blocks[name] = std::make_shared<orca::Block>(name);
}

void Orca::get_xyz(const std::string& xyzfile) {
    this->xyz.read_xyz_file(xyzfile);
    job.set_run("xyz_file", fs::absolute(xyzfile).string());

    this->new_block("coords");
    this->blocks["coords"]->data.push_back("CTyp xyz");
    this->blocks["coords"]->data.push_back("Charge 0");
    this->blocks["coords"]->data.push_back("Units Angs");
    this->blocks["coords"]->data.push_back("coords");
    for (const auto& item : xyz.atoms) {
        std::ostringstream tmp;
        tmp.setf(std::ios::fixed);
        tmp << std::setw(5)
            << item.name << " "
            << std::setprecision(9) << std::setw(15)
            << item.x << " "
            << std::setprecision(9) << std::setw(15)
            << item.y << " "
            << std::setprecision(9) << std::setw(15)
            << item.z;
        this->blocks["coords"]->data.push_back(
            tmp.str()
        );
    }
    this->blocks["coords"]->data.push_back("end");

    this->set_job_steps_default();
}

void Orca::set_job_steps_default() {
    job.steps.clear();
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << "cat > orca.inp<<EOF\n";
    step << this->to_string();
    step << "EOF\n";
    step << "$CMD_HEAD " << job.run_params["cmd"] << "\n";
    job.steps.push_back(step.str());
    step.clear();
}

void Orca::run(const std::string& directory) {
    job.run(directory);
}

} // namespace atomsciflow
