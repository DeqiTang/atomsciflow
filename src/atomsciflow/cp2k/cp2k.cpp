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

/// @file src/atomsciflow/cp2k/cp2k.cpp
/// @author deqi
/// Mail: deqi_tang@163.com
/// Created Time: Sun 31 Jan 2021 07:28:30 PM CST

#include "atomsciflow/cp2k/cp2k.h"

#include <iostream>
#include <fstream>
//#include <filesystem>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include "atomsciflow/server/submit_script.h"
#include "atomsciflow/remote/server.h"

namespace atomsciflow {

namespace fs = boost::filesystem;

Cp2k::Cp2k() {

    this->new_section("global");
    this->sections["global"].set_param("project", "cp2k_job");
    this->sections["global"].set_param("print_level", "low");
    this->sections["global"].set_param("run_type", "energy_force");

    this->new_section("force_eval");
    this->sections["force_eval"].set_param("method", "quickstep");

    Cp2kSection dft{"dft"};
    dft.set_param("basis_set_file_name", "basis_set");
    dft.set_param("potential_file_name", "gth_poetntials");

    auto& qs = dft.add_subsection("qs");
    qs.set_param("eps_default", "1.0e-14");

    auto& mgrid = dft.add_subsection("mgrid");
    mgrid.set_param("ngrids", 4);
    mgrid.set_param("cutoff", 100);
    mgrid.set_param("rel_cutoff", 60);

    auto& xc = dft.add_subsection("xc");
    auto& xc_functional = xc.add_subsection("xc_functional");
    xc_functional.section_parameter = "pbe";


    auto& scf = dft.add_subsection("scf");
    scf.set_param("scf_guess", "atomic");
    scf.set_param("eps_scf", "1.0e-7");
    scf.set_param("max_scf", 100);
    auto& diag = scf.add_subsection("diagonalization");
    diag.set_param("algorithm", "standard");

    auto& mixing = dft.add_subsection("mixing");
    mixing.set_param("method", "broyden_mixing");
    mixing.set_param("alpha", 0.4);
    mixing.set_param("nbroyden", 8);

    auto& print = dft.add_subsection("print");
    print.add_subsection("forces").section_parameter = "on";

    this->sections["force_eval"].add_subsection("dft", dft);

    job.set_run("cmd", "$CP2K_BIN");
    job.set_run("input", "cp2k.inp");
    job.set_run("output", "cp2k.out");

    job.set_run("script_name_head", "cp2k-run");
}

Cp2k::~Cp2k() {

}

void Cp2k::new_section(const std::string& name) {
    this->sections[name] = Cp2kSection{name};
}

std::string Cp2k::to_string() {
    std::string out = "";
    for (const auto& item : this->sections) {
        out += this->sections[item.first].to_string("  ") + "\n";
        out += "\n";
    }
    return out;
}

void Cp2k::get_xyz(const std::string &xyzfile) {
    this->xyz.read_xyz_file(xyzfile);
    this->set_subsys();
    job.set_run("xyz_file", fs::absolute(xyzfile).string());
    this->set_job_steps_default();
}

Cp2kSection& Cp2k::set_subsys(Xyz& xyz) {
    this->xyz = xyz;
    return this->set_subsys();
}

Cp2kSection& Cp2k::set_subsys() {
    auto& subsys = this->sections["force_eval"].add_subsection("subsys"); 
    auto& cell = subsys.add_subsection("cell");
    cell.set_param("a", this->xyz.cell[0]);
    cell.set_param("b", this->xyz.cell[1]);
    cell.set_param("c", this->xyz.cell[2]);
    
    auto& coord = subsys.add_subsection("coord");
    std::vector<std::vector<std::string>> matrix_str;
    for (const auto& atom : this->xyz.atoms) {
        matrix_str.push_back(std::vector<std::string>{
            atom.name,
            std::to_string(atom.x),
            std::to_string(atom.y),
            std::to_string(atom.z),
        });
    }
    coord.section_var.set("", matrix_str);
    return subsys;
}

void Cp2k::set_job_steps_default() {
    job.steps.clear();
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << boost::format("cat >%1%<<EOF\n") % job.run_params["input"];
    step << this->to_string();
    step << "EOF\n";
    step << boost::format("$CMD_HEAD %1% -in %2% | tee %3%  \n") % job.run_params["cmd"] % job.run_params["input"] % job.run_params["output"];
    job.steps.push_back(step.str());
    step.clear();
}

/**
 * @brief Cp2k::run The running entry for the calculation
 * @param directory The path to the directory where the calculation
 *  is running.
 * The running of the calculation is actually controlled
 * by the JobScheduler.
 */
void Cp2k::run(const std::string& directory) {
    job.run(directory);
}

} // namespace atomsciflow
