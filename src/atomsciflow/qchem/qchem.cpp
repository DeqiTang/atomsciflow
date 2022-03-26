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

/// @file src/atomsciflow/qchem/qchem.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 03:40:00 PM CST

#include "atomsciflow/qchem/qchem.h"

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

QChem::QChem() {

    new_section("rem");
    sections["rem"]->data.push_back("METHOD HF");
    sections["rem"]->data.push_back("BASIS 6-31g**");
    sections["rem"]->data.push_back("SOLVENT_METHOD Onsager");

    job.set_run_default("llhpc");
    job.set_run_default("pbs");
    job.set_run_default("bash");
    job.set_run_default("lsf_sz");
    job.set_run_default("lsf_sustc");
    job.set_run_default("cdcloud");

    job.set_run("cmd", "$QCHEM_BIN");
    job.set_run("script_name_head", "qchem-run");
}

QChem::~QChem() {

}


void QChem::new_section(const std::string& name) {
    this->sections[name] = std::make_shared<qchem::KeywordSection>(name);
}

std::string QChem::to_string() {
    std::string out = "";

    for (auto& item : this->sections) {
        out += item.second->to_string();
        out += "\n";
    }

    return out;
}

void QChem::get_xyz(const std::string& xyzfile) {
    this->xyz.read_xyz_file(xyzfile);
    job.set_run("xyz_file", fs::absolute(xyzfile).string());

    auto element_map = get_element_number_map();

    this->new_section("molecule");
    std::ostringstream tmp;
    tmp.setf(std::ios::fixed);
    tmp << "0 1";
    for (const auto& item : xyz.atoms) {
        tmp << std::setw(5)
            << item.name
            << std::setprecision(9) << std::setw(15)
            << item.x
            << std::setprecision(9) << std::setw(15)
            << item.y
            << std::setprecision(9) << std::setw(15)
            << item.z;
        this->sections["molecule"]->data.push_back(
            tmp.str()
        );
    }

    this->set_job_steps_default();
}

void QChem::set_job_steps_default() {
    job.steps.clear();
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << "cat > qchem.inp<<EOF\n";
    step << this->to_string();
    step << "EOF\n";
    step << "$CMD_HEAD " << job.run_params["cmd"] << "\n";
    job.steps.push_back(step.str());
    step.clear();
}

void QChem::run(const std::string& directory) {
    job.run(directory);
}

} // namespace atomsciflow
