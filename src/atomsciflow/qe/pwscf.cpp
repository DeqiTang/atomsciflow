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

/// @file src/atomsciflow/qe/pwscf.cpp
/// @author Deqi Tang
/// Mail: deqi_tang@163.com
/// Created Time: Tue 02 Feb 2021 05:56:20 PM CST

#include "atomsciflow/qe/pwscf.h"

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include "atomsciflow/server/submit_script.h"
#include "atomsciflow/remote/server.h"

namespace atomsciflow::qe {

namespace fs = boost::filesystem;

PwScf::PwScf() {
    this->namelists["control"] = qe::gen::control();
    this->namelists["system"] = qe::gen::system();
    this->namelists["electrons"] = qe::gen::electrons();
    this->namelists["ions"] = qe::gen::ions();
    this->namelists["cell"] = qe::gen::cell();

    job.set_run_default("bash");
    job.set_run_default("yh");
    job.set_run_default("llhpc");
    job.set_run_default("pbs");
    job.set_run_default("lsf_sz");
    job.set_run_default("lsf_sustc");
    job.set_run_default("cdcloud");
    job.set_run("cmd", "$QE_PW_BIN");
    job.set_run("runopt", "genrun");
    job.set_run("auto_level", "0");
    job.set_run("input", "pw.in");
    job.set_run("output", "pw.out");

    job.set_run("script_name_head", "pwscf-run");
}

PwScf::~PwScf() {

}

std::string PwScf::to_string() {
    return this->to_string("");    
}

std::string PwScf::to_string(std::string indent) {
    std::string out = "";
    for (const auto& item : this->namelists) {
        if (false == this->namelists[item.first].status) {
            continue;
        } else {
            out += this->namelists[item.first].to_string(indent);
            out += "\n";
        }
    }
    out += "\n";
    out += this->misc.to_string("angstrom");
    return out;
}

void PwScf::get_xyz(const std::string& xyzfile) {
    this->misc.xyz.read_xyz_file(xyzfile);
    job.set_run("xyz_file", fs::absolute(xyzfile).string());
    this->basic_setting_system();
    this->set_job_steps_default();
}

void PwScf::set_param(const std::string& namelist, std::string key, int value) {
    this->namelists[namelist].set_param(key, value);
}

void PwScf::set_param(const std::string& namelist, std::string key, double value) {
    this->namelists[namelist].set_param(key, value);
}

void PwScf::set_param(const std::string& namelist, std::string key, std::string value) {
    this->namelists[namelist].set_param(key, value);
}

void PwScf::set_param(const std::string& namelist, std::string key, std::vector<int> value) {
    this->namelists[namelist].set_param(key, value);
}

void PwScf::set_param(const std::string& namelist, std::string key, std::vector<double> value) {
    this->namelists[namelist].set_param(key, value);
}

void PwScf::set_param(const std::string& namelist, std::string key, std::vector<std::string> value) {
    this->namelists[namelist].set_param(key, value);
}

void PwScf::set_param(const std::string& namelist, std::string key, std::vector<std::vector<int>> value) {
    this->namelists[namelist].set_param(key, value);
}

void PwScf::set_param(const std::string& namelist, std::string key, std::vector<std::vector<double>> value) {
    this->namelists[namelist].set_param(key, value);
}

void PwScf::set_param(const std::string& namelist, std::string key, std::vector<std::vector<std::string>> value) {
    this->namelists[namelist].set_param(key, value);
}

void PwScf::set_kpoints(std::string kpoints_option, std::vector<int>& kpoints_mp, Kpath crystal_b) {
    this->misc.set_kpoints(kpoints_option, kpoints_mp, crystal_b);
}

void PwScf::basic_setting_ions(std::string calc = "relax") {
    if ("relax" == calc) {
        this->namelists["ions"].set_param("ion_dynamics", "bfgs");
    } else if ("md" == calc) {
        this->namelists["ions"].set_param("ion_dynamics", "verlet");
        this->namelists["ions"].set_param("ion_temperature", "not_controlled");
        this->namelists["ions"].set_param("tempw", 300);
    } else if ("vc-relax" == calc) {
        this->namelists["ions"].set_param("ion_dynamics", "bfgs");
    }
}

void PwScf::basic_setting_electrons() {
    this->namelists["electrons"].set_param("conv_thr", 1.0E-6);
    // plain mixing mode means Broyden mixing scheme
    // for charge density
    this->namelists["electrons"].set_param("mixing_mode", "plain");
    // the mixing factor for scf iteration
    this->namelists["electrons"].set_param("mixing_beta", 0.7E0);
    // number of previous iterations used in the mixing
    // procedure. More memoery is needed for larger mixing_ndim
    // if memoery is in shortage, reduce from 8 to 4.
    this->namelists["electrons"].set_param("mixing_ndim", 8);
    this->namelists["electrons"].set_param("diagonalization", "david");
}

void PwScf::basic_setting_control(std::string calc = "scf") {
    this->namelists["control"].set_param("calculation", calc);
    this->namelists["control"].set_param("prefix", "pwscf");
    if ("scf" == calc) {
        this->namelists["control"].set_param("outdir", "./tmp");
        this->namelists["control"].set_param("pseudo_dir", "./");
        this->namelists["control"].set_param("wf_collect", ".true.");
    } else if ("nscf" == calc) {
        this->namelists["control"].set_param("outdir", "./tmp");
        this->namelists["control"].set_param("pseudo_dir", "./tmp");
        this->namelists["control"].set_param("wf_collect", ".true.");
    } else if ("bands" == calc) {
        this->namelists["control"].set_param("outdir", "./tmp");
        this->namelists["control"].set_param("pseudo_dir", "./");
        this->namelists["control"].set_param("wf_collect", ".true.");
    } else if ("relax" == calc) {
        this->namelists["control"].set_param("outdir", "./tmp");
        this->namelists["control"].set_param("pseudo_dir", "./");
        this->namelists["control"].set_param("wf_collect", ".true.");
    } else if (calc == "md") {
        this->namelists["control"].set_param("outdir", "./tmp");
        this->namelists["control"].set_param("pseudo_dir", "./");
        this->namelists["control"].set_param("wf_collect", ".true.");
    } else if ("vc-relax" == calc) {
        this->namelists["control"].set_param("outdir", "./tmp");
        this->namelists["control"].set_param("pseudo_dir", "./");
        this->namelists["control"].set_param("wf_collect", ".true.");
    } else if ("vc-md" == calc) {
        this->namelists["control"].set_param("outdir", "./tmp");
        this->namelists["control"].set_param("pseudo_dir", "./");
        this->namelists["control"].set_param("wf_collect", ".true.");
    }
}

void PwScf::basic_setting_system() {
    this->namelists["system"].set_param("ibrav", 0);
    this->namelists["system"].set_param("nat", boost::lexical_cast<int>(misc.xyz.atoms.size()));
    this->namelists["system"].set_param("ntyp", misc.xyz.nspecies);

    this->namelists["system"].set_param("ecutwfc", 100);
    this->namelists["system"].set_param("input_dft", "PBE");
}

void PwScf::set_occupations(std::string occupations = "smearing", std::string smearing = "gaussian", double degauss=0.001) {
    this->namelists["system"].set_param("occupations", occupations);
    if ("smearing" == occupations) {
        this->namelists["system"].set_param("smearing", smearing);
        this->namelists["system"].set_param("degauss", degauss);
    }
    if ("tetrahedra" == occupations) {
        this->namelists["system"].set_param("smearing", "");
        this->namelists["system"].set_param("degauss", "");
    }
    if ("tetrahedra_lin" == occupations) {
        this->namelists["system"].set_param("smearing", "");
        this->namelists["system"].set_param("degauss", "");
    }
    if ("tetrahedra_opt" == occupations) {
        this->namelists["system"].set_param("smearing", "");
        this->namelists["system"].set_param("degauss", "");
    }
    if ("fixed" == occupations) {
        this->namelists["system"].set_param("smearing", "");
        this->namelists["system"].set_param("degauss", "");
    }
    if ("from_input" == occupations) {
        this->namelists["system"].set_param("smearing", "");
        this->namelists["system"].set_param("degauss", "");
    }
}

void PwScf::set_job_steps_default() {
    job.steps.clear();
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << boost::format("cat > %1%<<EOF\n") % job.run_params["input"];
    step << this->to_string();
    step << "EOF\n";
    step << boost::format("$CMD_HEAD %1% < %2% > %3%\n") % job.run_params["cmd"] % job.run_params["input"] % job.run_params["output"];
    job.steps.push_back(step.str());
    step.clear();
}

void PwScf::run(const std::string& directory) {
    job.run(directory);
}

} // namespcae atomsciflow::qe
