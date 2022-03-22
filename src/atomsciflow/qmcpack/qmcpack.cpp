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

/// @file src/atomsciflow/qmcpack/qmcpack.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Fri 11 Mar 2022 03:27:27 PM CST

#include "atomsciflow/qmcpack/qmcpack.h"

#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <iostream>

#include "atomsciflow/qmcpack/utils.h"
#include "atomsciflow/server/submit_script.h"
#include "atomsciflow/remote/server.h"

namespace atomsciflow {

namespace fs = boost::filesystem;

Qmcpack::Qmcpack() {

    this->input.root.put("simulation.project.<xmlattr>.series", 0);

    this->input.root.put("simulation.qmcsystem", "");
    this->input.root.put("simulation.qmcsystem.simulationcell", "");

    this->input.put_i_key("simulation.qmcsystem", 2, "particleset", "");

    this->input.root.put("simulation.qmcsystem.wavefunction.<xmlattr>.name", "psi0");
    this->input.root.put("simulation.qmcsystem.wavefunction.determinantset.slaterdeterminant", "");

    this->input.root.add("simulation.qmcsystem.wavefunction.jastrow", "");
    this->input.root.add("simulation.qmcsystem.wavefunction.jastrow", "");
    this->input.root.add("simulation.qmcsystem.wavefunction.jastrow", "");

    this->input.put_i_key("simulation.qmcsystem.hamiltonian", 0, "pairpot", "");
    this->input.put_i_key("simulation.qmcsystem.hamiltonian", 1, "pairpot", "");
    this->input.put_i_key("simulation.qmcsystem.hamiltonian", 2, "pairpot", "");

    this->input.put_attr("simulation.qmc", "method", "vmc");

    this->input.put_i_key("simulation.qmc", 0, "parameter", 100);
    this->input.put_i_key("simulation.qmc", 1, "parameter", 200);
    this->input.put_i_key("simulation.qmc", 2, "parameter", 300);

    this->input.put_i_key_attr("simulation.qmc", 0, "parameter", "name", 10);
    this->input.put_i_key_attr("simulation.qmc", 1, "parameter", "name", 20);
    this->input.put_i_key_attr("simulation.qmc", 2, "parameter", "name", 30);

    job.set_run("cmd", "$QMCPACK_BIN");
    job.set_run("input", "qmcpack.in");
    job.set_run("output", "qmcpack.out");

    job.set_run("script_name_head", "qmcpack-run");
};

void Qmcpack::get_xyz(const std::string &xyzfile) {
    this->xyz.read_xyz_file(xyzfile);
    job.set_run("xyz_file", fs::absolute(xyzfile).string());
    this->set_job_steps_default();
}

std::string Qmcpack::to_string() {
    return this->input.to_string();
}

void Qmcpack::set_job_steps_default() {
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

void Qmcpack::run(const std::string& directory) {
    job.run(directory);
}

} // namespace atomsciflow
