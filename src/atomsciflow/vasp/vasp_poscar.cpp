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

#include "atomsciflow/vasp/vasp_poscar.h"

#include <boost/algorithm/string.hpp>
#include <armadillo>
#include <iostream>
#include <vector>

#include "atomsciflow/utils/structure.h"
#include "atomsciflow/base/element.h"

namespace atomsciflow {

namespace ba = boost::algorithm;

VaspPoscar::VaspPoscar() {
    selective_dynamics = false;
}

void VaspPoscar::get_xyz(std::string xyzfile) {
    auto element_map = get_element_number_map();
    this->xyz.read_xyz_file(xyzfile);
    for (const auto& elem : this->xyz.elements_set) {
        this->elem_natom_in_number_order.push_back(std::pair<std::string, int>{elem, element_map[elem].number});
    }
    std::sort(
        this->elem_natom_in_number_order.begin(),
        this->elem_natom_in_number_order.end(),
        [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) -> bool {
            return a.second < b.second;
        }
    );
    for (auto& item : this->elem_natom_in_number_order) {
        item.second = 0;
        for (const auto& atom : this->xyz.atoms) {
            if (item.first == atom.name) {
                ++item.second;
            }
        }
    }
}

/**
 * @brief VaspPoscar::to_string
 * @param coordtype
 *  candidates:
 *      "direct" | "cartesian", ignoring upper or lower case
 * @return The vasp POSCAR as a string
 * Note:
 *  THe order of atoms in the gnerated POSCAR string might be
 *  different from the input xyz file. Because, the atoms in
 *  POSCAR is grouped considering the element type.
 */
std::string VaspPoscar::to_string(std::string coordtype = "Cartesian") {
    std::string out = "";
    auto cell = this->xyz.cell;
    out += "general comment\n";
    out += "1.0\n";
    for (const auto& vec : cell) {
        out += std::to_string(vec[0]);
        out += "\t";
        out += std::to_string(vec[1]);
        out += "\t";
        out += std::to_string(vec[2]);
        out += "\n";
    }
    for (const auto& item : this->elem_natom_in_number_order) {
        out += item.first;
        out += " ";
    }
    out += "\n";
    for (const auto& item : this->elem_natom_in_number_order) {
        out += std::to_string(item.second);
        out += " ";
    }
    out += "\n";
    if (this->selective_dynamics == true) {
        out += "Selective dynamics\n";
        if (ba::to_lower_copy(coordtype) == "cartesian") {
            out += "Cartesian\n";
            for (const auto& element_pair : this->elem_natom_in_number_order) {
                for (const auto& atom : this->xyz.atoms) {
                    if (element_pair.first == atom.name) {
                        out += std::to_string(atom.x);
                        out += "\t";
                        out += std::to_string(atom.y);
                        out += "\t";
                        out += std::to_string(atom.z);
//                        for (auto fix : atom.fix) {
//                            if (fix == true) {
//                                out += "\tF";
//                            } else if (fix == False) {
//                                out += "\tT";
//                            }
//                        }
                        out += "\n";
                    }
                }
            }
        } else if (ba::to_lower_copy(coordtype) == "direct") {

            std::vector<Atom> atoms_frac;
            atoms_cart_to_frac(atoms_frac, this->xyz.atoms, this->xyz.cell);

            out += "Direct\n";
            for (const auto& element_pair : this->elem_natom_in_number_order) {
                for (int k = 0; k < this->xyz.atoms.size(); k++) {
                    if (element_pair.first == this->xyz.atoms[k].name) {
                        out += std::to_string(atoms_frac[k].x);
                        out += std::to_string(atoms_frac[k].y);
                        out += std::to_string(atoms_frac[k].z);
//                        for (auto fix in this->xyz.atoms[k].fix) {
//                            if (fix == true) {
//                                out += "\tF";
//                            } else if (fix == false) {
//                                out += "\tT";
//                            }
//                        }
                        out += "\n";
                    }
                }
            }
        }
    } else if (false == this->selective_dynamics) {
        if (ba::to_lower_copy(coordtype) == "cartesian") {
            out += "Cartesian\n";
            for (const auto& item : this->elem_natom_in_number_order) {
                for (const auto& atom : this->xyz.atoms) {
                    if (atom.name == item.first) {
                        out += std::to_string(atom.x);
                        out += "\t";
                        out += std::to_string(atom.y);
                        out += "\t";
                        out += std::to_string(atom.z);
                        out += "\n";
                    }
                }
            }
        } else if (ba::to_lower_copy(coordtype) == "direct") {
            std::vector<Atom> atoms_frac;
            atoms_cart_to_frac(atoms_frac, this->xyz.atoms, this->xyz.cell);

            out += "Direct\n";
            for (const auto& item : this->elem_natom_in_number_order) {
                for (int k = 0; k < this->xyz.atoms.size(); k++) {
                    if (this->xyz.atoms[k].name == item.first) {
                        out += std::to_string(atoms_frac[k].x);
                        out += "\t";
                        out += std::to_string(atoms_frac[k].y);
                        out += "\t";
                        out += std::to_string(atoms_frac[k].z);
                        out += "\n";
                    }
                }
            }
        }
    } else {
        std::cout << "------------------------------------------------------------------------" << "\n";
        std::cout << "Warning: atomsciflow::Vasp::VaspPoscar\n";
        std::cout << "ifstatic could only be true or false\n";
        std::cout << "------------------------------------------------------------------------" << "\n";
        std::exit(1);
    }
    out += "\n";
    return out;
}

} // namespace atomsciflow
