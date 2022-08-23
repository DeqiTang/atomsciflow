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

    set_param("SystemName", "siesta-job");
    set_param("SystemLabel", "siesta-run");

    set_param("XC.functional", "GGA");
    set_param("XC.Authors", "PBE");
    set_param("DM.Tolerance", 1.0e-6);
    set_param("DM.MixingWeight", 0.2);
    set_param("DM.NumberPulay", 8);
    set_param("DM.AllowExtrapolation", "true");
    set_param("DM.UseSaveDM", "true");
    set_param("SolutionMethod", "diagon");
    set_param("MeshCutOff", "300 Ry");

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
            electrons += item.second.to_string("same-line", ""); // same-line and indent=""
            electrons += "\n";
        } else if (grouping.ions_incharge.find(item.first) != grouping.ions_incharge.end()) {
            ions += item.second.to_string("same-line", ""); // same-line and indent=""
            ions += "\n";
        } else {
            misc += item.second.to_string("same-line", ""); // same-line and indent=""
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

void Siesta::py_set_param(std::string key, int value) {
    this->set_param(key, value);
}

void Siesta::py_set_param(std::string key, double value) {
    this->set_param(key, value);
}

void Siesta::py_set_param(std::string key, std::string value) {
    this->set_param(key, value);
}

void Siesta::py_set_param(std::string key, std::vector<int> value) {
    this->set_param(key, value);
}

void Siesta::py_set_param(std::string key, std::vector<double> value) {
    this->set_param(key, value);
}

void Siesta::py_set_param(std::string key, std::vector<std::string> value) {
    this->set_param(key, value);
}

void Siesta::py_set_param(std::string key, std::vector<std::vector<int>> value) {
    this->set_param(key, value);
}

void Siesta::py_set_param(std::string key, std::vector<std::vector<double>> value) {
    this->set_param(key, value);
}

void Siesta::py_set_param(std::string key, std::vector<std::vector<std::string>> value) {
    this->set_param(key, value);
}

void Siesta::new_block(const std::string& name) {
    if (this->blocks.find(name) == this->blocks.end()) {
        this->blocks[name] = std::make_shared<siesta::Block>(name);
    }
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

    this->set_param("NumberOfSpecies", int(this->xyz.elements_set.size()));
    this->set_param("NumberOfAtoms", this->xyz.natom());

    this->set_param("AtomicCoordinatesFormat", "Ang");
    // this->set_param("AtomicCoordinatesFormat", "ScaledCartesian");
    this->set_param("LatticeConstant", "1.0 Ang");

    i = 0;
    for (const auto& item : elem_index_in_number_order) {
        this->set_block_data("ChemicalSpeciesLabel", item.second, i, 0);
        this->set_block_data("ChemicalSpeciesLabel", element_map[item.first].number, i, 1);
        this->set_block_data("ChemicalSpeciesLabel", item.first, i, 2);
        i++;
    }

    this->new_block("AtomicCoordinatesAndAtomicSpecies");
    i = 0;
    for (const auto& item : xyz.atoms) {
        set_block_data("AtomicCoordinatesAndAtomicSpecies", (boost::format("%1$10.8f") % item.x).str(), i, 0);
        set_block_data("AtomicCoordinatesAndAtomicSpecies", (boost::format("%1$10.8f") % item.y).str(), i, 1);
        set_block_data("AtomicCoordinatesAndAtomicSpecies", (boost::format("%1$10.8f") % item.z).str(), i, 2);
        set_block_data("AtomicCoordinatesAndAtomicSpecies", elem_index_map[item.name], i, 3);
        // add a comment for atom
        set_block_data(
            "AtomicCoordinatesAndAtomicSpecies", 
            (boost::format("# %1% %2%") % item.name % (i+1)).str(), 
            i, 4
        );
        i++;
    }

    i = 0;
    for (const auto& item : xyz.cell) {
        set_block_data("LatticeVectors", (boost::format("%1$10.8f")%item[0]).str(), i, 0);
        set_block_data("LatticeVectors", (boost::format("%1$10.8f")%item[1]).str(), i, 1);
        set_block_data("LatticeVectors", (boost::format("%1$10.8f")%item[2]).str(), i, 2);
        i++;    
    }

    i = 0;
    for (const auto& item : elem_index_in_number_order) {
        set_block_data("PAO.BasisSizes", item.first, i, 0);
        set_block_data("PAO.BasisSizes", "DZP", i, 1);
        i++;      
    }    
}

void Siesta::set_bandlines(Kpath& kpath) {
    set_param("BandLinesScale", "ReciprocalLatticeVectors");
    set_param("WriteKbands", "false");
    set_param("WriteBands", "false");

    set_block_data("BandLines", 1, 0, 0);
    set_block_data("BandLines", kpath.coords[0][0], 0, 1);
    set_block_data("BandLines", kpath.coords[0][1], 0, 2);
    set_block_data("BandLines", kpath.coords[0][2], 0, 3);
    set_block_data("BandLines", kpath.labels[0], 0, 4);
    
    for (int i = 1; i < kpath.links.size(); i++) {
        set_block_data(
            "BandLines", 
            kpath.links[i-1] == 0 ? 1 : kpath.links[i-1],
            i,
            0
        );
        set_block_data("BandLines", kpath.coords[i][0], i, 1);
        set_block_data("BandLines", kpath.coords[i][1], i, 2);
        set_block_data("BandLines", kpath.coords[i][2], i, 3);
        set_block_data("BandLines", kpath.labels[i], i, 4);
    }
}

void Siesta::run(const std::string& directory) {
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
