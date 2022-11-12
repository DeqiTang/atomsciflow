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

#include "atomsciflow/cp2k/neb.h"

#include <sstream>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>

namespace atomsciflow::cp2k {

namespace fs = boost::filesystem;

Neb::Neb() {
    set_param("global/run_type", "BAND");
    set_param("motion/band/band_type", "CI-NEB");
    set_param("motion/band/k_spring", 2.0e-2);
    set_param("motion/band/rotate_frames", "TRUE");
    set_param("motion/band/convergence_info(on)/each/band", 1);
    set_param("motion/band/energy(on)/each/band", 1);
    set_param("motion/band/optimize_band/diis/max_steps", 120);
    set_param("motion/band/program_run_info/initial_configuration_info", "TRUE");
    set_param("motion/print/trajectory/each/band", 1);
}

void Neb::set_images(const std::vector<std::string>& images) {
    for (auto& item : images) {
        Xyz xyz;
        xyz.read_xyz_file(item);
        this->images.push_back(xyz);
    }
}

void Neb::run(const std::string& directory) {

    int i = 0;
    for (const auto& item : this->images) {
        set_param(
            (boost::format("motion/band/replica[%1%]/coord_file_name") % i).str(), 
            fs::absolute(fs::path(directory) / fs::path(item.file).filename().string()).string()
        );
        i++;
    }

    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    for (const auto& item : this->images) {
        step << boost::format("cp %1% ${ABSOLUTE_WORK_DIR}\n") % item.file;
    }
    job.steps.push_back(step.str());
    step.clear();

    step.str("");
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << boost::format("cat >%1%<<EOF\n") % job.run_params["input"];
    step << this->to_string();
    step << "EOF\n";
    step << boost::format("$CMD_HEAD %1% -in %2% | tee -a %3%  \n") 
        % job.run_params["cmd"] 
        % job.run_params["input"]
        % job.run_params["output"]
        ;
    job.steps.push_back(step.str());
    step.clear();

    job.run(directory);
}

} // namespace atomsciflow::cp2k