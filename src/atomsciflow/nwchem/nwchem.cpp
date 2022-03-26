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

/// @file src/atomsciflow/nwchem/nwchem.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 03:28:12 PM CST

#include "atomsciflow/nwchem/nwchem.h"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <iomanip>

namespace atomsciflow {

namespace fs = boost::filesystem;

NWChem::NWChem() {

    this->non_directive = std::make_shared<VariableGroup>();

    set_param("Charge", 0);
    set_param("title", "NWChem calculation");

    job.set_run_default("llhpc");
    job.set_run_default("pbs");
    job.set_run_default("bash");
    job.set_run_default("lsf_sz");
    job.set_run_default("lsf_sustc");
    job.set_run_default("cdcloud");

    job.set_run("cmd", "$ASF_CMD_NWCHEM");
    job.set_run("script_name_head", "nwchem-run");    
}

void NWChem::get_xyz(const std::string& filepath) {
    this->xyz.read_xyz_file(filepath);
    job.set_run("xyz_file", fs::absolute(filepath).string());

    //TODO
}

std::string NWChem::to_string() {
    std::string out = "";
    return out;
}

void NWChem::new_directive(const std::string& name) {
    this->directives[name] = std::make_shared<nwchem::Directive>(name);
}

template <typename T>
void NWChem::set_param(const std::string& key, T value) {
    this->non_directive->set_param(key, value);
}

void NWChem::py_set_param(const std::string& key, int value) {
    this->set_param(key, value);
}

void NWChem::py_set_param(const std::string& key, double value) {
    this->set_param(key, value);
}

void NWChem::py_set_param(const std::string& key, std::string value) {
    this->set_param(key, value);
}

void NWChem::py_set_param(const std::string& key, std::vector<int> value) {
    this->set_param(key, value);
}

void NWChem::py_set_param(const std::string& key, std::vector<double> value) {
    this->set_param(key, value);
}

void NWChem::py_set_param(const std::string& key, std::vector<std::string> value) {
    this->set_param(key, value);
}

void NWChem::py_set_param(const std::string& key, std::vector<std::vector<int>> value) {
    this->set_param(key, value);
}

void NWChem::py_set_param(const std::string& key, std::vector<std::vector<double>> value) {
    this->set_param(key, value);
}

void NWChem::py_set_param(const std::string& key, std::vector<std::vector<std::string>> value) {
    this->set_param(key, value);
}

void NWChem::set_job_steps_default() {
    job.steps.clear();
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << "cat > nwchem.nw<<EOF\n";
    step << this->to_string();
    step << "EOF\n";
    step << "$CMD_HEAD " << job.run_params["cmd"] << "\n";
    job.steps.push_back(step.str());
    step.clear();
}

void NWChem::run(const std::string& directory) {
    job.run(directory);
}

// explicit template instantiation
template void NWChem::set_param<int>(const std::string& key, int);
template void NWChem::set_param<double>(const std::string& key, double);
template void NWChem::set_param<std::string>(const std::string& key, std::string);
template void NWChem::set_param<std::vector<int>>(const std::string& key, std::vector<int>);
template void NWChem::set_param<std::vector<double>>(const std::string& key, std::vector<double>);
template void NWChem::set_param<std::vector<std::string>>(const std::string& key, std::vector<std::string>);
template void NWChem::set_param<std::vector<std::vector<int>>>(const std::string& key, std::vector<std::vector<int>>);
template void NWChem::set_param<std::vector<std::vector<double>>>(const std::string& key, std::vector<std::vector<double>>);
template void NWChem::set_param<std::vector<std::vector<std::string>>>(const std::string& key, std::vector<std::vector<std::string>>);


} // namespace atomsciflow
