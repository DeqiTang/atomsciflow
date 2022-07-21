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

#include "atomsciflow/abinit/phonopy.h"

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "atomsciflow/abinit/utils.h"

namespace atomsciflow::abinit {

Phonopy::Phonopy() {
    this->job.set_run("phonopy_dim_x", 1);
    this->job.set_run("phonopy_dim_y", 1);
    this->job.set_run("phonopy_dim_z", 1);
}

Phonopy::~Phonopy() {

}

void Phonopy::run(const std::string& directory) {
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << "cat > " << this->files.main_in << "<<EOF\n";
    // when using the default this->datasets[0]->system->coordtype = "cartesian";,
    // the space group of the structure could not be parsed correctly by phonopy. 
    // So, we use "reduced" here.
    this->datasets[0]->system->coordtype = "reduced";
    step << this->to_string();
    step << "EOF\n";
    step << "cat > " << this->files.name << "<<EOF\n";
    step << this->files.to_string(*this->datasets[0]->system);
    step << "EOF\n";
    step << "cp";
    for (const auto& item : this->datasets[0]->system->xyz.elements_set) {
        if (this->files.pseudo_ext == ".psp8") {
            step << " " << (fs::path(config.get_pseudo_pot_dir()["abinit"]) / "pbe_s_sr/" / (item + ".psp8")).string();
        } else if (this->files.pseudo_ext == ".GGA_PBE-JTH.xml") {
            step << " " << (fs::path(config.get_pseudo_pot_dir()["abinit"]) / "JTH-PBE-atomicdata-1.1/ATOMICDATA/" / (item + ".GGA_PBE-JTH.xml")).string();
        } else {
            step << " " << (fs::path(config.get_pseudo_pot_dir()["abinit"]) / "pbe_s_sr/" / (item + ".psp8")).string();
        }
    }
    step << " ./\n";
    job.steps.push_back(step.str());
    step.str("");
    step.clear();

    step << boost::format("phonopy --abinit -c %1% -d --dim=\'%2% %3% %4%\'\n")
        % this->files.main_in
        % job.run_params["phonopy_dim_x"]
        % job.run_params["phonopy_dim_y"]
        % job.run_params["phonopy_dim_z"]
        ;
    job.steps.push_back(step.str());
    step.str("");
    step.clear();

    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << "cat > abinit-input-part-1.in<<EOF\n";
    step << "ndtset ";
    step << std::to_string(this->ndtset);
    step << "\n";
    step << dataset_n_title(0);
    step << "\n";
    this->datasets[0]->system->status = false;
    this->datasets[0]->ions->status = false;
    this->datasets[1]->ions->status = false;
    step << this->datasets[0]->to_string();
    for (int i = 1; i < this->ndtset + 1; i++) {
        this->datasets[i]->system->status = false;
        step << dataset_n_title(i);
        step << "\n";
        step << this->datasets[i]->to_string();
    }
    step << "EOF\n";
    job.steps.push_back(step.str());
    step.str("");
    step.clear();

    step << boost::format("for item in supercell-*.in\n");
    step << "do\n";
    step << "mkdir run-${item/.in/}\n";
    step << "cat abinit-input-part-1.in ${item} > run-${item/.in/}/abinit.in\n";
    step << boost::format("cp *%1% run-${item/.in/}/\n") % this->files.pseudo_ext;
    step << "cp abinit.files run-${item/.in/}/\n";
    step << "done\n";
    job.steps.push_back(step.str());
    step.str("");
    step.clear();

    step << boost::format("for item in run-supercell-*\n");
    step << "do\n";
    step << "cd ${item}\n";
    step << boost::format("$CMD_HEAD %1% < abinit.files\n")
        % job.run_params["cmd"];
    step << "cd ../\n";
    step << "done\n";
    job.steps.push_back(step.str());
    step.str("");
    step.clear();

    job.run(directory);
}

} // namespace atomsciflow::abinit
