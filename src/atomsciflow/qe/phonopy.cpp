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

#include "atomsciflow/qe/phonopy.h"

#include <iostream>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

namespace atomsciflow::qe {

Phonopy::Phonopy() {
    this->job.set_run("phonopy_dim_x", 1);
    this->job.set_run("phonopy_dim_y", 1);
    this->job.set_run("phonopy_dim_z", 1);

    this->set_param("control", "tprnfor", ".true.");
    this->set_param("control", "tstress", ".true.");

    this->namelists["ions"].status = false;
    this->namelists["cell"].status = false;
}

Phonopy::~Phonopy() {

}

void Phonopy::run(const std::string& directory) {
    
    job.steps.clear();
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << boost::format("cat > %1%<<EOF\n") % job.run_params["input"];
    step << this->to_string();
    step << "EOF\n";
    job.steps.push_back(step.str());
    step.clear();
    step.str("");

    for (const auto& item : this->xyz.elements_set) {
        step << "# pseudopotential file for element: " << item << "\n";
        step << boost::format("for item in %1%/*\n") 
            % (fs::path(config.get_pseudo_pot_dir()["qe"]) / "SSSP_efficiency_pseudos").string();
        step << "do\n";
        if (item.size() == 1) {
            step << boost::format("if [[ ${item} =~ /[%1%|%2%][.|_] ]]\n")
                % boost::to_upper_copy(item)
                % boost::to_lower_copy(item);
        } else {
            step << boost::format("if [[ ${item} =~ /[%1%|%2%][%3%|%4%][.|_] ]]\n")
                % boost::to_upper_copy(boost::lexical_cast<std::string>(item[0]))
                % boost::to_lower_copy(boost::lexical_cast<std::string>(item[0]))
                % boost::to_upper_copy(boost::lexical_cast<std::string>(item[1]))
                % boost::to_lower_copy(boost::lexical_cast<std::string>(item[1]));
        }
        step << "then\n";
        step << "cp ${item} ${ABSOLUTE_WORK_DIR}/\n";
        step << "fi\n";
        step << "done\n";
    }
    job.steps.push_back(step.str());
    step.clear();
    step.str("");

    step << boost::format("phonopy --qe -c %1% -d --dim=\'%2% %3% %4%\'\n")
        % job.run_params["input"]
        % job.run_params["phonopy_dim_x"]
        % job.run_params["phonopy_dim_y"]
        % job.run_params["phonopy_dim_z"]
        ;
    job.steps.push_back(step.str());
    step.str("");
    step.clear();

    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << "cat > pwscf-input-part-1.in<<EOF\n";
    std::vector<std::string> namelists_order;
    namelists_order.push_back("control");
    namelists_order.push_back("system");
    namelists_order.push_back("electrons");
    namelists_order.push_back("ions");
    namelists_order.push_back("cell");
    this->set_param(
        "system", 
        "nat", 
        this->xyz.natom() 
            * boost::lexical_cast<int>(job.run_params["phonopy_dim_x"])
            * boost::lexical_cast<int>(job.run_params["phonopy_dim_y"])
            * boost::lexical_cast<int>(job.run_params["phonopy_dim_z"])
    );
    for (const auto& item : namelists_order) {
        if (false == this->namelists[item].status) {
            continue;
        } else {
            step << this->namelists[item].to_string("");
            step << "\n";
        }
    }
    step << "\n";
    step << "EOF\n";
    job.steps.push_back(step.str());
    step.clear();
    step.str("");

    step << boost::format("for item in supercell-*.in\n");
    step << "do\n";
    step << "cat pwscf-input-part-1.in ${item} > run-${item}\n";
    step << "cat >>run-${item}<<EOF\n";
    step << "\n\n";
    step << this->cards["k_points"]->to_string();
    step << "EOF\n";
    step << "done\n";
    job.steps.push_back(step.str());
    step.clear();
    step.str("");

    step << boost::format("for item in run-supercell-*.in\n");
    step << "do\n";
    step << boost::format("$CMD_HEAD %1% < ${item} > ${item}.out\n") 
        % job.run_params["cmd"];
    step << "done\n";
    job.steps.push_back(step.str());
    step.str("");
    step.clear();

    job.run(directory);
}

} // namespace atomsciflow::qe
