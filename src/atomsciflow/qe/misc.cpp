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

/// @file src/atomsciflow/qe/misc.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Tue 08 Mar 2022 09:25:55 PM CST

#include "atomsciflow/qe/misc.h"

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <armadillo>

#include "atomsciflow/base/element.h"
#include "atomsciflow/utils/structure.h"

namespace atomsciflow {

namespace fs = boost::filesystem;

PwScfPseudo::PwScfPseudo() {
    this->directory = fs::absolute(fs::path("./")).string(); // default dir to put pseudo files
}

std::string PwScfPseudo::to_string(Xyz& xyz) {
    auto element_map = get_element_number_map();

    std::string out = "";
    out += "ATOMIC_SPECIES\n";
    std::vector<std::string> all_file;
    for (const auto& p : fs::directory_iterator(this->directory)) {
        all_file.emplace_back(p.path().string());
    }
    for (const auto& element : xyz.elements_set) {
        for (const auto& item : all_file) {
            out += element;
            out += element_map[element].mass;
            out+= item;
        }
    }
    return out;
}

PwScfMisc::PwScfMisc() {
    this->cell_params["cell_dynamics"] = "";
    this->cell_params["press"] = "";
    this->cell_params["wmass"] = "";
    this->cell_params["cell_factor"] = "";
    this->cell_params["press_conv_thr"] = "";
    this->cell_params["cell_dofree"] = "";

    this->kpoints_option = "automatic";
    this->kpoints_mp.clear();
    kpoints_mp.push_back(1);
    kpoints_mp.push_back(1);
    kpoints_mp.push_back(1);
    kpoints_mp.push_back(0);
    kpoints_mp.push_back(0);
    kpoints_mp.push_back(0);

    this->ifstatic = true;
    this->atomic_forces_status = false;
}

std::string PwScfMisc::to_string(std::string coordtype="angstrom") {
    std::string out = "";
    out += this->pseudo.to_string(this->xyz);
    out += "\n";
    auto cell = this->xyz.cell;
    out += "CELL_PARAMETERS angstrom\n";
    for (int i = 0; i < 3; i++) {
        out += std::to_string(cell[i][0]);
        out += " ";
        out += std::to_string(cell[i][1]);
        out += " ";
        out += std::to_string(cell[i][2]);
        out += "\n";
    }
    out += "\n";
    if (coordtype == "angstrom") {
        out += "ATOMIC_POSITIONS angstrom\n";
        if (this->ifstatic == true) {
            for (const auto& atom : this->xyz.atoms) {
                out += atom.name;
                out += "\t";
                out += std::to_string(atom.x);
                out += "\t";
                out += std::to_string(atom.y);
                out += "\t";
                out += std::to_string(atom.z);
                out += "\n";
            }
        } else if (false == this->ifstatic) {
            for (const auto& atom : this->xyz.atoms) {
                out += atom.name;
                out += "\t";
                out += std::to_string(atom.x);
                out += "\t";
                out += std::to_string(atom.y);
                out += "\t";
                out += std::to_string(atom.z);
                out += "\n";
//                for (auto fix : atom.fix) {
//                    if (fix == true) {
//                        out += "\t0";
//                    } else if (fix == false) {
//                        out += "\t1";
//                    }
//                }
                out += "\n";
            }
        }
        out += "\n";
    } else  if ("crystal" == coordtype) {

        std::vector<Atom> atoms_frac;
        atoms_cart_to_frac(atoms_frac, this->xyz.atoms, this->xyz.cell);

        out += "ATOMIC_POSITIONS crystal\n";
        if (true == this->ifstatic) {
            for (int k = 0; k < this->xyz.atoms.size(); k++) {
                out += this->xyz.atoms[k].name;
                out += std::to_string(atoms_frac[k].x);
                out += "\t";
                out += std::to_string(atoms_frac[k].y);
                out += "\t";
                out += std::to_string(atoms_frac[k].z);
                out += "\n";
            }
        } else if (false == this->ifstatic) {
            for (int k = 0; k < this->xyz.atoms.size(); k++) {
                out += this->xyz.atoms[k].name;
                out += std::to_string(atoms_frac[k].x);
                out += "\t";
                out += std::to_string(atoms_frac[k].y);
                out += "\t";
                out += std::to_string(atoms_frac[k].z);
                out += "\n";
//                for (auto fix : this->xyz.atoms[k].fix) {
//                    if (fix == true) {
//                        out += "\t0";
//                    } else if (fix == false) {
//                        out += "\t1";
//                    }
//                }
                out += "\n";
            }
        }
        out += "\n";
    }

    out += this->to_string_kpoints();

    if (true == this->atomic_forces_status) {
        out += this->to_string_atomic_forces();
    }
    return out;
}

std::string PwScfMisc::to_string_kpoints() {
    std::string out = "";
    if ("automatic" == this->kpoints_option) {
        out += (boost::format("K_POINTS %1%\n") % this->kpoints_option).str();
        out += (boost::format("%1% %2% %3% %4% %5% %6%\n")
            % this->kpoints_mp[0]
            % this->kpoints_mp[1]
            % this->kpoints_mp[2]
            % this->kpoints_mp[3]
            % this->kpoints_mp[4]
            % this->kpoints_mp[5]
        ).str();
    } else if ("gamma" == this->kpoints_option) {
        out += "K_POINTS gamma\n";
    } else if ("crystal_b" == this->kpoints_option) {
        out += (boost::format("K_POINTS %1%\n") % this->kpoints_option).str();
        out += std::to_string(this->crystal_b.coords.size());
        for (int i = 0; i < this->crystal_b.coords.size(); i ++) {
            out += std::to_string(this->crystal_b.coords[i][0]);
            out += " ";
            out += std::to_string(this->crystal_b.coords[i][1]);
            out += " ";
            out += std::to_string(this->crystal_b.coords[i][2]);
            out += " ";
            out += std::to_string(this->crystal_b.links[i] != 0 ? this->crystal_b.links[i] : 0);
            out += " #";
            out += this->crystal_b.labels[i];
            out += "\n";
        }
    } else if ("tpiba_b" == this->kpoints_option) {

    }
    return out;
}

/// \brief PwScfMisc::set_kpoints
/// \param option
///  candidates:
///     "automatic" | "gamma" | "crystal_b"
/// \param kpoints_mp
/// \param crystal_b an instance of atomsciflow::Kpath
void PwScfMisc::set_kpoints(std::string option, std::vector<int>& kpoints_mp, Kpath crystal_b) {
    if ("automatic" == option) {
        this->kpoints_option = option;
        this->kpoints_mp = kpoints_mp;
        return;
    }
    if ("gamma" == option) {
        this->kpoints_option = option;
        return;
    }
    if ("crystal_b" == option) {
        this->kpoints_option = option;
        this->crystal_b = crystal_b;
        return;
    }
}

std::string PwScfMisc::to_string_atomic_forces() {
    std::string out = "";
    out += "ATOMIC_FORCES\n";
    for (int i = 0; i < this->xyz.atoms.size(); i++) {
        out += this->xyz.atoms[i].name;
        out += " ";
        out += std::to_string(this->atomic_forces[i][0]);
        out += " ";
        out += std::to_string(this->atomic_forces[i][1]);
        out += " ";
        out += std::to_string(this->atomic_forces[i][2]);
        out += "\n";
    }
    out += '\n';
    return out;
}

void PwScfMisc::set_atomic_forces(std::string pressure, std::string direction) {
    if ("" == pressure || "" == direction) {
        this->atomic_forces_status = false;
        return;
    } else {
        this->atomic_forces_status = true;
    }

    if ("x" == direction) {
        double area = std::sqrt(std::pow(this->xyz.cell[1][0], 2) + std::pow(this->xyz.cell[1][1], 2) + std::pow(this->xyz.cell[1][2], 2)) * std::sqrt(std::pow(this->xyz.cell[2][0], 2) + std::pow(this->xyz.cell[2][1], 2) + std::pow(this->xyz.cell[2][2], 2)); // in unit of Anstrom^2
        double force = area * 1.0e-20 * std::atof(pressure.c_str()) / (4.119358925e8);
        arma::mat tmp(this->xyz.atoms.size(), 3);
        tmp.col(0) = force;
        this->atomic_forces = arma::conv_to<std::vector<std::vector<double>>>::from(tmp);
    }
}

void PwScfMisc::basic_setting(bool ifstatic = true) {
    this->ifstatic = ifstatic;
}

} //namespace atomsciflow
