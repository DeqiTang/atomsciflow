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

/// @file src/atomsciflow/exciting/exciting.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 03:02:37 PM CST

#include "atomsciflow/exciting/exciting.h"

#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <iostream>

#include "atomsciflow/qmcpack/utils.h"
#include "atomsciflow/server/submit_script.h"
#include "atomsciflow/remote/server.h"

namespace atomsciflow {

namespace fs = boost::filesystem;

Exciting::Exciting() {
    input.root.put("input.title", "Exciting Calculation");

    input.root.put("input.groundstate.<xmlattr>.CoreRelativity", "dirac");
    input.root.put("input.groundstate.<xmlattr>.autokpt", "true");
    
    input.root.put("input.groundstate.solver.<xmlattr>.type", "Lapack");

    job.set_run("cmd", "$ASF_CMD_EXCITING");
    job.set_run("input", "exciting.in");
    job.set_run("output", "exciting.out");

    job.set_run("script_name_head", "exciting-run");
}

void Exciting::get_xyz(const std::string &xyzfile) {
    this->xyz.read_xyz_file(xyzfile);
    job.set_run("xyz_file", fs::absolute(xyzfile).string());
    this->set_job_steps_default();
}

std::string Exciting::to_string() {
    return this->input.to_string();
}

void Exciting::set_job_steps_default() {
    job.steps.clear();
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << boost::format("cat > %1%<<EOF\n") % job.run_params["input"];
    step << this->input.to_string();
    step << "EOF\n";

    step << boost::format("${CMD_HEAD} %1% %2%\n") % job.run_params["cmd"] % job.run_params["input"];
    job.steps.push_back(step.str());
    step.clear();
}

void Exciting::run(const std::string& directory) {
    job.run(directory);
}

} // namespace atomsciflow