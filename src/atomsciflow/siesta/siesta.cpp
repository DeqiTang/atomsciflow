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

/// @file src/atomsciflow/siesta/siesta.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Tue 22 Mar 2022 10:18:25 PM CST

#include "atomsciflow/siesta/siesta.h"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <iomanip>

#include "atomsciflow/siesta/utils.h"
#include "atomsciflow/server/submit_script.h"
#include "atomsciflow/remote/server.h"
#include "atomsciflow/base/element.h"

namespace atomsciflow {

namespace fs = boost::filesystem;

Siesta::Siesta() {

    this->non_block = std::make_shared<VariableGroup>();

    set_param("XC.functional", "GGA");
    set_param("DM.Tolerance", 1.0e-6);
    set_param("DM.MixingWeight", 0.1);
    set_param("DM.NumberPulay", 8);
    set_param("DM.AllowExtrapolation", "true");
    set_param("DM.UseSaveDM", "true");
    set_param("SolutionMethod", "diagon");
    set_param("MeshCutOff", "300 Ry");

    job.set_run_default("llhpc");
    job.set_run_default("pbs");
    job.set_run_default("bash");
    job.set_run_default("lsf_sz");
    job.set_run_default("lsf_sustc");
    job.set_run_default("cdcloud");

    job.set_run("input", "siesta.fdf");
    job.set_run("output", "siesta.out");
    job.set_run("cmd", "$ASF_CMD_SIESTA");
    job.set_run("script_name_head", "siesta-run");
}

std::string Siesta::to_string() {
    std::string out = "";

    std::string electrons = siesta_group_title("Electrons related params");
    std::string ions = siesta_group_title("Ions related params");
    std::string misc = siesta_group_title("Miscellaneous params");

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
    
    out += electrons;
    out += ions;
    out += misc;
    out += "\n";
    
    out += siesta_group_title("Blocks input");
    out += "\n";
    for (auto& item : this->blocks) {
        out += item.second->to_string();
        out += "\n";
    }

    return out;
}

template <typename T>
void Siesta::set_param(std::string key, T value) {
    this->non_block->set_param(key, value);
}

void Siesta::new_block(const std::string& name) {
    this->blocks[name] = std::make_shared<siesta::Block>(name);
}

void Siesta::get_xyz(const std::string& xyzfile) {
    this->xyz.read_xyz_file(xyzfile);
    job.set_run("xyz_file", fs::absolute(xyzfile).string());

    auto element_map = get_element_number_map();

    for (const auto& elem : this->xyz.elements_set) {
        this->elem_index_in_number_order.push_back(std::pair<std::string, int>{elem, element_map[elem].number});
    }
    std::sort(
        this->elem_index_in_number_order.begin(),
        this->elem_index_in_number_order.end(),
        [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) -> bool {
            return a.second < b.second;
        }
    );
    int i = 0;
    for (auto& item : this->elem_index_in_number_order) {
        item.second = i+1;
        i++;
        elem_index_map[item.first] = item.second;
    }

    this->new_block("ChemicalSpeciesLabel");
    for (const auto& item : elem_index_in_number_order) {
        this->blocks["ChemicalSpeciesLabel"]->data.push_back(
            (boost::format("%1% %2% %3%")
            % item.second % element_map[item.first].mass % item.first).str()
        );
    }

    this->new_block("AtomicCoordinatesAndAtomicSpecies");
    int j = 0;
    for (const auto& item : xyz.atoms) {
        j++;
        // this->blocks["AtomicCoordinatesAndAtomicSpecies"]->data.push_back(
        //     (boost::format("%1$.6f %2$.6f %3$.6f %4% # %5% %6%")
        //     % item.x % item.y % item.z % elem_index_map[item.name]
        //     % item.name % j).str()
        // );
        std::ostringstream tmp;
        tmp.setf(std::ios::fixed);
        tmp << std::setprecision(9) << std::setw(15)
            << item.x << " "
            << std::setprecision(9) << std::setw(15)
            << item.y << " "
            << std::setprecision(9) << std::setw(15)
            << item.z << " "
            << std::setw(5)
            << elem_index_map[item.name] << " # "
            << item.name << " "
            << j;
        this->blocks["AtomicCoordinatesAndAtomicSpecies"]->data.push_back(
            tmp.str()
        );
    }

    new_block("LatticeVectors");
    for (const auto& item : xyz.cell) {
        std::ostringstream tmp;
        tmp.setf(std::ios::fixed);
        tmp << std::setprecision(9) << std::setw(15)
            << item[0] << " "
            << std::setprecision(9) << std::setw(15)
            << item[1] << " "
            << std::setprecision(9) << std::setw(15)
            << item[2];
        this->blocks["LatticeVectors"]->data.push_back(
            tmp.str()
        );        
    }

    new_block("PAO.BasisSizes");
    for (const auto& item : elem_index_in_number_order) {
        std::ostringstream tmp;
        tmp.setf(std::ios::fixed);
        tmp << std::setw(5)
            << item.first 
            << " " << "DZP";
        this->blocks["PAO.BasisSizes"]->data.push_back(
            tmp.str()
        );         
    }

    this->set_job_steps_default();
}

void Siesta::set_job_steps_default() {
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
    step << "$CMD_HEAD " << job.run_params["cmd"] << " < " << job.run_params["input"] << " > " << job.run_params["output"] << "\n";
    job.steps.push_back(step.str());
    step.clear();
}

void Siesta::run(const std::string& directory) {
    job.run(directory);
}

// explicit template instantiation
template void Siesta::set_param<int>(std::string key, int);
template void Siesta::set_param<double>(std::string key, double);
template void Siesta::set_param<std::string>(std::string key, std::string);
template void Siesta::set_param<std::vector<int>>(std::string key, std::vector<int>);
template void Siesta::set_param<std::vector<double>>(std::string key, std::vector<double>);
template void Siesta::set_param<std::vector<std::string>>(std::string key, std::vector<std::string>);
template void Siesta::set_param<std::vector<std::vector<int>>>(std::string key, std::vector<std::vector<int>>);
template void Siesta::set_param<std::vector<std::vector<double>>>(std::string key, std::vector<std::vector<double>>);
template void Siesta::set_param<std::vector<std::vector<std::string>>>(std::string key, std::vector<std::vector<std::string>>);

} // namespace atomsciflow
