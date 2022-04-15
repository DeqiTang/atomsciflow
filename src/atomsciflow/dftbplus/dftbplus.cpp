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

/// @file src/atomsciflow/dftbplus/dftbplus.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 02:58:19 PM CST

#include "atomsciflow/dftbplus/dftbplus.h"

#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <iostream>

#include "atomsciflow/server/submit_script.h"
#include "atomsciflow/remote/server.h"

namespace atomsciflow {

namespace fs = boost::filesystem;

DftbPlus::DftbPlus() {
    
    job.set_run("cmd", "$ASF_CMD_DFTBPLUS");
    job.set_run("input", "dftbplus.in");

    job.set_run("script_name_head", "dftbplus-run");
}

void DftbPlus::get_xyz(const std::string &xyzfile) {
    this->xyz.read_xyz_file(xyzfile);
    job.set_run("xyz_file", fs::absolute(xyzfile).string());
    this->set_job_steps_default();
}

std::string DftbPlus::to_string() {
    std::string out = "";
    //TODO
    return out;
}

void DftbPlus::set_job_steps_default() {
    job.steps.clear();
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << boost::format("cat > %1%<<EOF\n") % job.run_params["input"];
    step << this->to_string();
    step << "EOF\n";

    step << boost::format("${CMD_HEAD} %1% %2%\n") % job.run_params["cmd"] % job.run_params["input"];
    job.steps.push_back(step.str());
    step.clear();
}

void DftbPlus::run(const std::string& directory) {
    job.run(directory);
}

} // namespace atomsciflow