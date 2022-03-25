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

/// @file src/atomsciflow/siesta/siesta.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Tue 22 Mar 2022 10:18:25 PM CST

#include "atomsciflow/siesta/siesta.h"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <iomanip>

#include "atomsciflow/siesta/utils.h"
#include "atomsciflow/server/submit_script.h"
#include "atomsciflow/remote/server.h"

namespace atomsciflow {

namespace fs = boost::filesystem;

Siesta::Siesta() {

    new_group("electrons");
    set_param("electrons", "XC.functional", "GGA");
    set_param("electrons", "DM.Tolerance", 1.0e-6);
    set_param("electrons", "DM.MixingWeight", 0.1);
    set_param("electrons", "DM.NumberPulay", 8);
    set_param("electrons", "DM.AllowExtrapolation", "true");
    set_param("electrons", "DM.UseSaveDM", "true");
    set_param("electrons", "SolutionMethod", "diagon");
    set_param("electrons", "MeshCutOff", "300 Ry");

    job.set_run_default("llhpc");
    job.set_run_default("pbs");
    job.set_run_default("bash");
    job.set_run_default("lsf_sz");
    job.set_run_default("lsf_sustc");
    job.set_run_default("cdcloud");

    job.set_run("cmd", "$SIESTA_BIN");
    job.set_run("script_name_head", "siesta-run");
}

std::string Siesta::to_string() {
    std::string out = "";
    for (const auto& item : this->groups) {
        out += siesta_group_title(item.first);
        out += this->groups[item.first]->to_string();
    }
    return out;
}

void Siesta::new_group(const std::string &name) {
    if (this->groups.find(name) == this->groups.end()) {
        this->groups[name] = std::make_shared<VariableGroup>();
    }
}

template <typename T>
void Siesta::set_param(const std::string& group, std::string key, T value) {
    this->groups[group]->set_param(key, value);
}

void Siesta::get_xyz(const std::string& xyzfile) {
    this->xyz.read_xyz_file(xyzfile);
    job.set_run("xyz_file", fs::absolute(xyzfile).string());
    //TODO
    this->set_job_steps_default();
}

void Siesta::set_job_steps_default() {
    job.steps.clear();
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << "cat > siesta.fdf<<EOF\n";
    step << this->to_string();
    step << "EOF\n";
    step << "$CMD_HEAD " << job.run_params["cmd"] << "\n";
    job.steps.push_back(step.str());
    step.clear();
}

void Siesta::run(const std::string& directory) {
    job.run(directory);
}

// explicit template instantiation
template void Siesta::set_param<int>(const std::string& group, std::string key, int);
template void Siesta::set_param<double>(const std::string& group, std::string key, double);
template void Siesta::set_param<std::string>(const std::string& group, std::string key, std::string);
template void Siesta::set_param<std::vector<int>>(const std::string& group, std::string key, std::vector<int>);
template void Siesta::set_param<std::vector<double>>(const std::string& group, std::string key, std::vector<double>);
template void Siesta::set_param<std::vector<std::string>>(const std::string& group, std::string key, std::vector<std::string>);
template void Siesta::set_param<std::vector<std::vector<int>>>(const std::string& group, std::string key, std::vector<std::vector<int>>);
template void Siesta::set_param<std::vector<std::vector<double>>>(const std::string& group, std::string key, std::vector<std::vector<double>>);
template void Siesta::set_param<std::vector<std::vector<std::string>>>(const std::string& group, std::string key, std::vector<std::vector<std::string>>);

} // namespace atomsciflow
