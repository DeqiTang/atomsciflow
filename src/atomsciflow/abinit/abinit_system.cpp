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

/// @file src/atomsciflow/abinit/abinit_system.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Wed 09 Mar 2022 08:33:01 PM CST

#include "atomsciflow/abinit/abinit_system.h"

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include "atomsciflow/abinit/utils.h"

namespace atomsciflow {

/**
 * @brief AbinitSystem::get_xyz
 * @param xyzfile the xyz file path
 * While getting the information from the xyz file, the member
 * variable this->elem_typat is adjusted for further generation
 * of Abinit input string.
 *
 * The Abinit software requires that the number of columns in the
 * input file is less than 264, which should be considered when
 * input the typat variable, as there might be a large number of
 * atoms to be listed. Taking into account this situation, it's
 * guranteed there is less than 30 atoms listed in one line for
 * typat.
 */
void AbinitSystem::get_xyz(const std::string& xyzfile) {
    this->xyz.read_xyz_file(xyzfile);
    int i = 0;
    for (const auto& element : this->xyz.elements_set) {
        this->elem_typat[element] = i + 1;
        i++;
    }
}

std::string AbinitSystem::to_string(int n = 0) {
    auto element_number_map = get_element_number_map();
    std::string out = "";
    out += group_setting_title("system related setting");
    out += "\n";
    if (n > 0) {
        out += (boost::format("acell%1% 1 1 1 angstrom\n\n") % n).str();
        out += (boost::format("rprim%1%\n") % n).str();
        for (int i = 0; i < 3; i++) {
            out += std::to_string(this->xyz.cell[i][0]) + " " + std::to_string(this->xyz.cell[i][1]) + " " + std::to_string(this->xyz.cell[i][2]) +"\n";
        }
        out += "\n";
        out += "ntypat" + std::to_string(n) + " " + std::to_string(this->xyz.nspecies) + "\n\n";
        out += "natom" + std::to_string(n) + " " + std::to_string(this->xyz.natom()) + "\n\n";

        out += "typat" + std::to_string(n) + "\n";
        int i = 0;
        for (const auto& atom : this->xyz.atoms) {
            out += std::to_string(this->elem_typat[atom.name]) + " ";
            if (i % 30 == 29) {
                out += "\n";
            }
            i += 1;
        }
        out += "\n\n";
        out += (boost::format("znucl%1%\n") % n).str();
        for (const auto& element :this->elem_typat) {
            out += std::to_string(element_number_map[element.first].number);
            out += " ";
        }
        out += "\n\n";
        if ("cartesian" == this->coordtype) {
            out += (boost::format("xcart%1%\n") % (n > 0 ? std::to_string(n) : "")).str();
            for (const auto& atom : this->xyz.atoms) {
                out += std::to_string(atom.x) + " " + std::to_string(atom.y) + " " + std::to_string(atom.z) + "\n";
            }
            out += "Angstrom\n";
        } else if (this->coordtype == "reduced") {
            std::vector<Atom> atoms_frac;
            atoms_cart_to_frac(atoms_frac, this->xyz.atoms, this->xyz.cell);
            out += "xred" + std::to_string(n) + "\n";
            for (int k = 0; k < atoms_frac.size(); k++) {
                out += std::to_string(atoms_frac[k].x) + " " + std::to_string(atoms_frac[k].y) + " " + std::to_string(atoms_frac[k].z) + "\n";
            }
        }
        out += "\n";
    } else {
        out += "acell 1 1 1 angstrom\n\n";
        out += "rprim\n";
        for (int i = 0; i < 3; i++) {
            out += std::to_string(this->xyz.cell[i][0]) + " " + std::to_string(this->xyz.cell[i][1]) + " " + std::to_string(this->xyz.cell[i][2]) +"\n";
        }
        out += "\n";
        out += "ntypat ";
        out += std::to_string(this->xyz.nspecies) + "\n\n";
        out += "natom " + std::to_string(this->xyz.natom()) + "\n\n";
        out += "typat \n";
        int i = 0;
        for (const auto& atom : this->xyz.atoms) {
            out += std::to_string(this->elem_typat[atom.name]) + " ";
            if (i % 30 == 29) {
                out += "\n";
            }
            i += 1;
        }
        out += "\n\n";
        out += "znucl\n";
        for (const auto& element :this->xyz.elements_set) {
            out += std::to_string(element_number_map[element].number);
            out += " ";
        }
        out += "\n\n";
        if ("cartesian" == this->coordtype) {
            out += (boost::format("xcart%1%\n") % (n > 0 ? std::to_string(n) : "")).str();
            for (const auto& atom : this->xyz.atoms) {
                out += std::to_string(atom.x) + " " + std::to_string(atom.y) + " " + std::to_string(atom.z) + "\n";
            }
            out += "Angstrom\n";
        } else if ("reduced" == this->coordtype) {
            std::vector<Atom> atoms_frac;
            atoms_cart_to_frac(atoms_frac, this->xyz.atoms, this->xyz.cell);
            out += "xred\n";
            for (int k = 0; k < atoms_frac.size(); k++) {
                out += std::to_string(atoms_frac[k].x) + " " + std::to_string(atoms_frac[k].y) + " " + std::to_string(atoms_frac[k].z) + "\n";
            }
        }
        out += "\n";
    }
    return out;
}

} // namespace atomsciflow
