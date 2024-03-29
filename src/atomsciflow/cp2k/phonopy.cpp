/************************************************************************
MIT License

Copyright (c) 2022 Deqi Tang

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

#include "atomsciflow/cp2k/phonopy.h"

#include <iostream>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

namespace atomsciflow::cp2k {

Phonopy::Phonopy() {

    this->job.set_run("phonopy_dim_x", 1);
    this->job.set_run("phonopy_dim_y", 1);
    this->job.set_run("phonopy_dim_z", 1);

    this->set_param("force_eval/stress_tensor", "ANALYTICAL");
    this->set_param("force_eval/dft/mgrid/cutoff", 300);
    this->set_param("force_eval/dft/mgrid/rel_cutoff", 100);
    this->set_param("force_eval/dft/qs/eps_default", 1.0e-14);
    this->set_param("force_eval/dft/scf/eps_scf", 1.0e-7);
    this->set_param("force_eval/dft/scf/max_scf", 50);
}

void Phonopy::run(const std::string& directory) {
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << boost::format("cat >%1%<<EOF\n") % job.run_params["input"];
    step << this->to_string();
    step << "EOF\n";
    job.steps.push_back(step.str());
    step.str("");
    step.clear();

    step << boost::format("phonopy --cp2k -c %1% -d --dim=\'%2% %3% %4%\'\n")
        % job.run_params["input"]
        % job.run_params["phonopy_dim_x"]
        % job.run_params["phonopy_dim_y"]
        % job.run_params["phonopy_dim_z"]
        ;
    job.steps.push_back(step.str());
    step.str("");
    step.clear();

    std::vector<std::string> vec_str;
    boost::split(vec_str, job.run_params["input"], boost::is_any_of("."));
    step << boost::format("for inp in %1%-supercell-*.inp\n") % vec_str[0];
    step << "do\n";
    step << boost::format("$CMD_HEAD %1% -in %2% | tee -a %3% \n")
        % job.run_params["cmd"]
        % "${inp}"
        % "${inp}.out"
        ;
    step << "done\n";
    job.steps.push_back(step.str());
    step.str("");
    step.clear();

    // step << "cat >phonopy-run.txt<<EOF\n"
    //     << job.run_params["xyz_file"]
    //     << "\n"
    //     << "EOF\n"
    //     ;
    // job.steps.push_back(step.str());
    // step.str("");
    // step.clear();

    job.run(directory);
}

} // namespace atomsciflow::cp2k
