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

/// @file src/atomsciflow/vasp/vasp.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Fri 04 Mar 2022 04:08:27 PM CST

#include "atomsciflow/vasp/vasp.h"

#include <fstream>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#include "atomsciflow/server/submit_script.h"
#include "atomsciflow/remote/server.h"

namespace atomsciflow {

namespace fs = boost::filesystem;

Vasp::Vasp() {
    incar = std::make_shared<VaspIncar>();
    poscar = std::make_shared<VaspPoscar>();
    kpoints = std::make_shared<VaspKpoints>();

    incar->set_runtype("static");
    incar->basic_setting();

    job.set_run_default("bash");
    job.set_run_default("yh");
    job.set_run_default("llhpc");
    job.set_run_default("pbs");
    job.set_run_default("lsf_sz");
    job.set_run_default("lsf_sustc");
    job.set_run_default("cdcloud");
    job.set_run("cmd", "$ASF_CMD_VASP_STD");

    job.set_run("script_name_head", "vasp-run");
}

Vasp::~Vasp() {
}

void Vasp::get_xyz(const std::string& xyzfile) {
    this->poscar->get_xyz(xyzfile);
    job.set_run("xyz_file", fs::absolute(xyzfile).string());
    this->set_job_steps_default();
}

/// \brief Vasp::set_params
/// \param params
/// \param runtype the type of calculation
///     candidates;
///     'static' | 'opt' | 'phonopy' | 'phonon | 'neb' |
///     'md' | 'custom' |
void Vasp::set_params(const std::map<std::string, std::string>& params, const std::string& runtype) {
    if ("static" == runtype) {
        for (const auto& item : params) {
            if (limit.static_allowed.find(item.first) != limit.static_allowed.end()) {
                this->incar->set_param(item.first, item.second);
            }
        }
    } else if ("opt" == runtype) {
        for (const auto& item : params) {
            if (limit.opt_allowed.find(item.first) != limit.opt_allowed.end()) {
                this->incar->set_param(item.first, item.second);
            }
        }
    } else if ("phonopy" == runtype) {
        for (const auto& item : params) {
            if (limit.phonopy_allowed.find(item.first) != limit.phonopy_allowed.end()) {
                this->incar->set_param(item.first, item.second);
            }
        }
    } else if ("phonon"  == runtype) {
        for (const auto& item : params) {
            if (limit.phonon_allowed.find(item.first) != limit.phonon_allowed.end()) {
                this->incar->set_param(item.first, item.second);
            }
        }
    } else if ("neb" == runtype) {
        for (const auto& item : params) {
            if (limit.neb_allowed.find(item.first) != limit.neb_allowed.end()) {
                this->incar->set_param(item.first, item.second);
            }
        }
    } else if ("md" == runtype) {
        for (const auto& item : params) {
            if (limit.md_allowed.find(item.first) != limit.md_allowed.end()) {
                this->incar->set_param(item.first, item.second);
            }
        }
    } else if ("custom" == runtype) {
        for (const auto& item : params) {
            this->incar->set_param(item.first, item.second);
        }
    } else {
        //
    }
    return;
}

void Vasp::set_kpoints(const std::vector<int>& kpoints_mp = {1, 1, 1, 0, 0, 0}, const std::string& option="automatic", Kpath* kpath=nullptr) {
    this->kpoints->set_kpoints(kpoints_mp, option, kpath);
}

void Vasp::set_job_steps_default() {
    job.steps.clear();
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << "cat >INCAR<<EOF\n";
    step << this->incar->to_string();
    step << "EOF\n";
    step << "cat >KPOINTS<<EOF\n";
    step << this->kpoints->to_string();
    step << "EOF\n";
    step << "cat >POSCAR<<EOF\n";
    step << this->poscar->to_string("cartesian");
    step << "EOF\n";
    step << "$CMD_HEAD " << job.run_params["cmd"] << "\n";
    job.steps.push_back(step.str());
    step.clear();
}

void Vasp::run(const std::string& directory) {
    job.run(directory);
}

} // namespace atomsciflow
