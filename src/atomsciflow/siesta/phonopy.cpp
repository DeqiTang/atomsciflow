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

#include "atomsciflow/siesta/phonopy.h"

#include <iostream>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "atomsciflow/siesta/utils.h"

namespace atomsciflow::siesta {

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
    step << "cat >" << job.run_params["input"] << "<<EOF\n";
    step << this->to_string();
    step << "EOF\n";
    step << "cp";
    for (const auto& item : this->xyz.elements_set) {
        step << " " << (fs::path(config.get_pseudo_pot_dir()["siesta"]) / "abinit-ncpp" / (item + ".psf")).string();
    }
    step << " ./\n";
    // step << "$CMD_HEAD " << job.run_params["cmd"] << " < " << job.run_params["input"] << " > " << job.run_params["output"] << "\n";
    job.steps.push_back(step.str());
    step.str("");
    step.clear();


    step << boost::format("phonopy --siesta -c %1% -d --dim=\'%2% %3% %4%\'\n")
        % job.run_params["input"]
        % job.run_params["phonopy_dim_x"]
        % job.run_params["phonopy_dim_y"]
        % job.run_params["phonopy_dim_z"]
        ;
    job.steps.push_back(step.str());
    step.str("");
    step.clear();

    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << "cat > siesta-input-part-1.fdf<<EOF\n";

    std::string electrons = siesta_group_title("Electrons related params");
    std::string ions = siesta_group_title("Ions related params");
    std::string misc = siesta_group_title("Miscellaneous params");

    this->non_block->set_status("NumberOfAtoms", false);
    this->non_block->set_status("AtomicCoordinatesFormat", false);
    this->non_block->set_status("LatticeConstant", false);

    for (auto& item : this->non_block->params) {
        if (false == item.second.status) {
            continue;
        }
        if (grouping.electrons_incharge.find(item.first) != grouping.electrons_incharge.end()) {
            electrons += item.second.to_string();
            electrons += "\n";
        } else if (grouping.ions_incharge.find(item.first) != grouping.ions_incharge.end()) {
            ions += item.second.to_string();
            ions += "\n";
        } else {
            misc += item.second.to_string();
            misc += "\n";
        }
    }
    
    step << electrons;
    step << ions;
    step << misc;
    step << "\n";
    
    step <<  siesta_group_title("Blocks input");
    step << "\n";
    for (auto& item : this->blocks) {
        if (item.first == "LatticeVectors") {
            continue;
        }
        if (item.first == "AtomicCoordinatesAndAtomicSpecies") {
            continue;
        }
        step << item.second->to_string();
        step << "\n";
    }
    step << "EOF\n";
    job.steps.push_back(step.str());
    step.clear();
    step.str("");

    step << boost::format("for item in supercell-*.fdf\n");
    step << "do\n";
    step << "mkdir run-${item/.fdf/}\n";
    step << "cat siesta-input-part-1.fdf ${item} > run-${item/.fdf/}/run-${item}\n";
    step << "cat >>run-${item/.fdf/}/run-${item}<<EOF\n";
    step << "\n";
    step << "EOF\n";
    step << "cp *.psf run-${item/.fdf/}/\n";
    step << "done\n";
    job.steps.push_back(step.str());
    step.clear();
    step.str("");

    step << boost::format("for item in run-supercell-*\n");
    step << "do\n";
    step << "cd ${item}\n";
    step << boost::format("$CMD_HEAD %1% < ${item}.fdf | tee ${item}.fdf.out\n")
        % job.run_params["cmd"];
    step << "cd ../\n";
    step << "done\n";
    job.steps.push_back(step.str());
    step.str("");
    step.clear();

    job.run(directory);
}

} // namespace atomsciflow::siesta
