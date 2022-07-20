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

#include "atomsciflow/vasp/phonopy.h"

#include <iostream>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

namespace atomsciflow::vasp {

Phonopy::Phonopy() {
    this->job.set_run("phonopy_dim_x", 1);
    this->job.set_run("phonopy_dim_y", 1);
    this->job.set_run("phonopy_dim_z", 1);
}

Phonopy::~Phonopy() {

}

void Phonopy::run(const std::string& directory) {
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
    step << "cat";
    for (const auto& item : this->poscar->elem_natom_in_number_order) {
        step << " " << (fs::path(this->config.get_pseudo_pot_dir()["vasp"]) / "PAW_PBE" / item.first / "POTCAR").string();
    }
    step << " >POTCAR\n";

    step << boost::format("phonopy -d --dim=\'%1% %2% %3%\'\n")
        % job.run_params["phonopy_dim_x"]
        % job.run_params["phonopy_dim_y"]
        % job.run_params["phonopy_dim_z"]
        ;
    job.steps.push_back(step.str());
    step.str("");
    step.clear();

    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << boost::format("for item in POSCAR-*\n");
    step << "do\n";
    step << "mkdir run-supercell-${item/POSCAR-/}\n";
    step << "cat ${item} > run-supercell-${item/POSCAR-/}/POSCAR\n";
    step << "cp INCAR run-supercell-${item/POSCAR-/}/\n";
    step << "cp KPOINTS run-supercell-${item/POSCAR-/}/\n";
    step << "cp POTCAR run-supercell-${item/POSCAR-/}/\n";
    step << "done\n";
    job.steps.push_back(step.str());
    step.clear();
    step.str("");

    step << boost::format("for item in run-supercell-*\n");
    step << "do\n";
    step << "cd ${item}\n";
    step << boost::format("$CMD_HEAD %1%\n") % job.run_params["cmd"];
    step << "cd ../\n";
    step << "done\n";
    job.steps.push_back(step.str());
    step.str("");
    step.clear();

    job.run(directory);
}

} // namespace atomsciflow::vasp
