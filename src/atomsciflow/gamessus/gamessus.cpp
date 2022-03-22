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

/// @file src/atomsciflow/gamessus/gamessus.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Fri 11 Mar 2022 09:48:47 AM CST

#include "atomsciflow/gamessus/gamessus.h"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include "atomsciflow/gamessus/utils.h"
#include "atomsciflow/base/element.h"
#include "atomsciflow/server/submit_script.h"
#include "atomsciflow/remote/server.h"

namespace atomsciflow {

namespace fs = boost::filesystem;

GamessUS::GamessUS() {

    this->job.set_run("cmd", "$GAMESSUS_BIN");

    this->job.set_run_default("llhpc");
    this->job.set_run_default("pbs");
    this->job.set_run_default("bash");
    this->job.set_run_default("lsf_sz");
    this->job.set_run_default("lsf_sustc");
    this->job.set_run_default("cdcloud");

    job.set_run("script_name_head", "gamessus-run");
}

GamessUS::~GamessUS() {
    for (const auto& item : this->groups) {
        delete item.second;
    }
}

void GamessUS::new_group(const std::string &name) {
    if (this->groups.find(name) == this->groups.end()) {
        this->groups[name] = new VariableGroup{};
    }
}

std::string GamessUS::to_string() {
    std::string out;
    for (const auto& item : this->groups) {
        out += group_title(item.first);
        out += this->groups[item.first]->to_string();
    }
    return out;
}

void GamessUS::get_xyz(const std::string& xyzfile) {
    this->xyz.read_xyz_file(xyzfile);
    job.set_run("xyz_file", fs::absolute(xyzfile).string());

    this->new_group("data");

    auto element_map = get_element_number_map();
    this->set_param("data", "Cnv", 2);
    std::string atm;
    int i = 0;
    for (const auto& item : this->xyz.atoms) {
        atm = "";
        atm += boost::lexical_cast<std::string>(element_map[item.name].mass);
        atm += " ";
        atm += std::to_string(item.x);
        atm += " ";
        atm += std::to_string(item.y);
        atm += " ";
        atm += std::to_string(item.z);
        this->set_param("data", item.name + std::to_string(i+1), atm);
        i += 0;
    }
    this->set_job_steps_default();
}

void GamessUS::set_param(const std::string& group, std::string key, int value) {
    this->groups[group]->set_param(key, value);
}

void GamessUS::set_param(const std::string& group, std::string key, double value) {
    this->groups[group]->set_param(key, value);
}

void GamessUS::set_param(const std::string& group, std::string key, std::string value) {
    this->groups[group]->set_param(key, value);
}

void GamessUS::set_param(const std::string& group, std::string key, std::vector<int> value) {
    this->groups[group]->set_param(key, value);
}

void GamessUS::set_param(const std::string& group, std::string key, std::vector<double> value) {
    this->groups[group]->set_param(key, value);
}

void GamessUS::set_param(const std::string& group, std::string key, std::vector<std::string> value) {
    this->groups[group]->set_param(key, value);
}

void GamessUS::set_param(const std::string& group, std::string key, std::vector<std::vector<int>> value) {
    this->groups[group]->set_param(key, value);
}

void GamessUS::set_param(const std::string& group, std::string key, std::vector<std::vector<double>> value) {
    this->groups[group]->set_param(key, value);
}

void GamessUS::set_param(const std::string& group, std::string key, std::vector<std::vector<std::string>> value) {
    this->groups[group]->set_param(key, value);
}

void GamessUS::set_job_steps_default() {
    job.steps.clear();
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << "cat > gamessus.inp<<EOF\n";
    step << this->to_string();
    step << "EOF\n";
    step << "$CMD_HEAD " << job.run_params["cmd"] << "\n";
    job.steps.push_back(step.str());
    step.clear();
}

void GamessUS::run(const std::string& directory) {
    job.run(directory);
}

} // namespace atomsciflow
