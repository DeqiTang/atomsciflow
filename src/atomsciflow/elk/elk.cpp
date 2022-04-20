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

/// @file src/atomsciflow/elk/elk.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Wed 02 Mar 2022 02:39:17 PM CST

#include "atomsciflow/elk/elk.h"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include "atomsciflow/base/element.h"
#include "atomsciflow/server/submit_script.h"
#include "atomsciflow/remote/server.h"

namespace atomsciflow {

namespace fs = boost::filesystem;

Elk::Elk() {

    job.set_run_default("bash");
    job.set_run_default("llhpc");
    job.set_run_default("pbs");
    job.set_run_default("bash");
    job.set_run_default("lsf_sz");
    job.set_run_default("lsf_sustc");
    job.set_run_default("cdcloud");
    job.set_run("cmd", "$ELK_BIN");
    job.set_run("runopt", "genrun");
    job.set_run("auto_level", "0");

    job.set_run("cmd", "$ASF_CMD_ELK");
    job.set_run("script_name_head", "elk-run");
}

Elk::~Elk() {
    for (const auto& item : this->blocks) {
        delete item.second;
    }
}

void Elk::new_block(const std::string &name) {
    if (this->blocks.find(name) == this->blocks.end()) {
        this->blocks[name] = new Block{};
    }
}

std::string Elk::to_string() {
    std::string out;
    for (const auto& item : this->blocks) {
        out += this->blocks[item.first]->to_string();
    }
    return out;
}

void Elk::get_xyz(std::string xyzfile) {
    //std::cout << "Elk::get_xyz starting!" << "\n";

    this->xyz.read_xyz_file(xyzfile);
    job.set_run("xyz_file", fs::absolute(xyzfile).string());

    this->new_block("data");

    auto element_map = get_element_number_map();
    this->set_param("data", "Cnv", 2);
    std::string atm;
    for (const auto& item : this->xyz.atoms) {
        atm = "";
        atm += std::to_string(element_map[item.name].mass);
        atm += " ";
        atm += std::to_string(item.x);
        atm += " ";
        atm += std::to_string(item.y);
        atm += " ";
        atm += std::to_string(item.z);
        this->set_param("data", item.name, atm);
    }
    this->set_job_steps_default();
}

template <typename T>
void Elk::set_param(const std::string& group, std::string key, T value) {
    this->blocks[group]->set_param(key, value);
}

void Elk::py_set_param(const std::string& group, std::string key, int value) {
    this->set_param(group, key, value);
}

void Elk::py_set_param(const std::string& group, std::string key, double value) {
    this->set_param(group, key, value);
}

void Elk::py_set_param(const std::string& group, std::string key, std::string value) {
    this->set_param(group, key, value);
}

void Elk::py_set_param(const std::string& group, std::string key, std::vector<int> value) {
    this->set_param(group, key, value);
}

void Elk::py_set_param(const std::string& group, std::string key, std::vector<double> value) {
    this->set_param(group, key, value);
}

void Elk::py_set_param(const std::string& group, std::string key, std::vector<std::string> value) {
    this->set_param(group, key, value);
}

void Elk::py_set_param(const std::string& group, std::string key, std::vector<std::vector<int>> value) {
    this->set_param(group, key, value);
}

void Elk::py_set_param(const std::string& group, std::string key, std::vector<std::vector<double>> value) {
    this->set_param(group, key, value);
}

void Elk::py_set_param(const std::string& group, std::string key, std::vector<std::vector<std::string>> value) {
    this->set_param(group, key, value);
}

void Elk::set_job_steps_default() {
    job.steps.clear();
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << "cat > elk.in<<EOF\n";
    step << this->to_string();
    step << "EOF\n";

    step << "$CMD_HEAD " << this->job.run_params["cmd"] << "\n";
    job.steps.push_back(step.str());
    step.clear();
}

/// @param directory a place for all the generated files
void Elk::run(const std::string& directory) {
    job.run(directory);
}

// explicit template instantiation
template void Elk::set_param<int>(const std::string& group, std::string key, int);
template void Elk::set_param<double>(const std::string& group, std::string key, double);
template void Elk::set_param<std::string>(const std::string& group, std::string key, std::string);
template void Elk::set_param<std::vector<int>>(const std::string& group, std::string key, std::vector<int>);
template void Elk::set_param<std::vector<double>>(const std::string& group, std::string key, std::vector<double>);
template void Elk::set_param<std::vector<std::string>>(const std::string& group, std::string key, std::vector<std::string>);
template void Elk::set_param<std::vector<std::vector<int>>>(const std::string& group, std::string key, std::vector<std::vector<int>>);
template void Elk::set_param<std::vector<std::vector<double>>>(const std::string& group, std::string key, std::vector<std::vector<double>>);
template void Elk::set_param<std::vector<std::vector<std::string>>>(const std::string& group, std::string key, std::vector<std::vector<std::string>>);

} // namespace atomsciflow
