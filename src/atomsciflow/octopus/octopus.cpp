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

/// @file src/atomsciflow/octopus/octopus.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 02:51:53 PM CST

#include "atomsciflow/octopus/octopus.h"

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

Octopus::Octopus() {

    this->non_block = std::make_shared<VariableGroup>();

    set_param("CalculationMode", "gs");
    set_param("ExtraStates", 1);
    set_param("Spacing", 0.5);
    set_param("UnitsOutput", "eV_Angstrom");

    job.set_run_default("llhpc");
    job.set_run_default("pbs");
    job.set_run_default("bash");
    job.set_run_default("lsf_sz");
    job.set_run_default("lsf_sustc");
    job.set_run_default("cdcloud");

    job.set_run("cmd", "$ASF_CMD_OCTOPUS");
    job.set_run("input", "inp");
    job.set_run("script_name_head", "octopus-run");
}

Octopus::~Octopus() {

}

std::string Octopus::to_string() {
    std::string out = "";

    for (auto& item : this->non_block->params) {
        if (false == item.second.status) {
            continue;
        }
        out += (boost::format("%1% = %2%") % item.second.key % item.second.as<std::string>()).str();
        out += "\n";
    }
    out += "\n";
    for (auto& item : this->blocks) {
        out += item.second->to_string();
        out += "\n";
    }

    return out;
}

template <typename T>
void Octopus::set_param(const std::string& key, T value) {
    this->non_block->set_param(key, value);
}

void Octopus::py_set_param(const std::string& key, int value) {
    this->set_param(key, value);
}

void Octopus::py_set_param(const std::string& key, double value) {
    this->set_param(key, value);
}

void Octopus::py_set_param(const std::string& key, std::string value) {
    this->set_param(key, value);
}

void Octopus::py_set_param(const std::string& key, std::vector<int> value) {
    this->set_param(key, value);
}

void Octopus::py_set_param(const std::string& key, std::vector<double> value) {
    this->set_param(key, value);
}

void Octopus::py_set_param(const std::string& key, std::vector<std::string> value) {
    this->set_param(key, value);
}

void Octopus::py_set_param(const std::string& key, std::vector<std::vector<int>> value) {
    this->set_param(key, value);
}

void Octopus::py_set_param(const std::string& key, std::vector<std::vector<double>> value) {
    this->set_param(key, value);
}

void Octopus::py_set_param(const std::string& key, std::vector<std::vector<std::string>> value) {
    this->set_param(key, value);
}

void Octopus::new_block(const std::string& name) {
    this->blocks[name] = std::make_shared<octopus::Block>(name);
}

void Octopus::get_xyz(const std::string& xyzfile) {
    this->xyz.read_xyz_file(xyzfile);
    job.set_run("xyz_file", fs::absolute(xyzfile).string());

    auto element_map = get_element_number_map();

    new_block("LatticeVectors");
    for (const auto& item : xyz.cell) {
        std::ostringstream tmp;
        tmp.setf(std::ios::fixed);
        tmp << std::setprecision(9) << std::setw(12)
            << item[0] << " | "
            << std::setprecision(9) << std::setw(12)
            << item[1] << " | "
            << std::setprecision(9) << std::setw(12)
            << item[2];
        this->blocks["LatticeVectors"]->data.push_back(
            tmp.str()
        );        
    }
    new_block("LatticeParameters");
    std::ostringstream tmp;
    tmp.setf(std::ios::fixed);
    tmp << std::setprecision(9) << std::setw(6)
        << 1 << " | "
        << std::setprecision(9) << std::setw(6)
        << 1 << " | "
        << std::setprecision(9) << std::setw(6)
        << 1;
    this->blocks["LatticeParameters"]->data.push_back(
        tmp.str()
    );

    this->new_block("Coordinates");
    for (const auto& item : xyz.atoms) {
        std::ostringstream tmp;
        tmp.setf(std::ios::fixed);
        tmp << std::setw(5)
            << "\'" << item.name << "\'" << " | "
            << std::setprecision(9) << std::setw(15)
            << item.x << " | "
            << std::setprecision(9) << std::setw(15)
            << item.y << " | "
            << std::setprecision(9) << std::setw(15)
            << item.z;
        this->blocks["Coordinates"]->data.push_back(
            tmp.str()
        );
    }

    this->set_job_steps_default();
}

void Octopus::set_job_steps_default() {
    job.steps.clear();
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << boost::format("cat >%1%<<EOF\n") % job.run_params["input"];
    step << this->to_string();
    step << "EOF\n";
    step << "$CMD_HEAD " << job.run_params["cmd"] << "\n";
    job.steps.push_back(step.str());
    step.clear();
}

void Octopus::run(const std::string& directory) {
    job.run(directory);
}

// explicit template instantiation
template void Octopus::set_param<int>(const std::string& key, int);
template void Octopus::set_param<double>(const std::string& key, double);
template void Octopus::set_param<std::string>(const std::string& key, std::string);
template void Octopus::set_param<std::vector<int>>(const std::string& key, std::vector<int>);
template void Octopus::set_param<std::vector<double>>(const std::string& key, std::vector<double>);
template void Octopus::set_param<std::vector<std::string>>(const std::string& key, std::vector<std::string>);
template void Octopus::set_param<std::vector<std::vector<int>>>(const std::string& key, std::vector<std::vector<int>>);
template void Octopus::set_param<std::vector<std::vector<double>>>(const std::string& key, std::vector<std::vector<double>>);
template void Octopus::set_param<std::vector<std::vector<std::string>>>(const std::string& key, std::vector<std::vector<std::string>>);

} // namespace atomsciflow
