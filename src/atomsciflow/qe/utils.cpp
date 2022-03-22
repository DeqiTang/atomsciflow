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

/// @file src/atomsciflow/qe/utils.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Tue 08 Mar 2022 10:34:49 PM CST

#include "atomsciflow/qe/utils.h"

#include "atomsciflow/variable/variable.h"
#include <set>

namespace atomsciflow {

std::string qe_variable_to_string(Variable& variable) {
    std::set<std::string> pwscf_character_var;

    pwscf_character_var.insert("prefix");
    pwscf_character_var.insert("outdir");
    pwscf_character_var.insert( "wfcdir");
    pwscf_character_var.insert( "pseudo_dir");
    pwscf_character_var.insert( "calculation");
    pwscf_character_var.insert("occupations");
    pwscf_character_var.insert( "smearing");
    pwscf_character_var.insert( "vdw_corr");
    pwscf_character_var.insert( "U_projection_type");
    pwscf_character_var.insert("input_dft");
    pwscf_character_var.insert( "exxdiv_treatment");
    pwscf_character_var.insert( "diagonalization");
    pwscf_character_var.insert( "ion_dynamics");
    pwscf_character_var.insert("pot_extrapolation");
    pwscf_character_var.insert( "wfc_extrapolation");
    pwscf_character_var.insert( "ion_temperature");
    pwscf_character_var.insert("cell_dofree");
    pwscf_character_var.insert( "mixing_mode");
    pwscf_character_var.insert( "efield_phase");
    pwscf_character_var.insert( "cell_dynamics");
    pwscf_character_var.insert("verbosity");
    pwscf_character_var.insert( "title");
    pwscf_character_var.insert( "disk_io");

    if (false == variable.status) {
        return "";
    }
    if (0 == variable.value.size()) {
        return "";
    }
    std::string out = "";
    if (0 == variable.value.size()) {
        return out + variable.key;
    }

    std::set<std::string> special;

    special.insert("starting_magnetization");
    special.insert("Hubbard_U");
    special.insert("Hubbard_J0");
    special.insert("Hubbard_alpha");
    special.insert("Hubbard_beta");

    if (special.find(variable.key) != special.end()) {
        for (int i = 0; i < variable.as<std::vector<std::string>>().size(); i ++) {
            out += variable.key;
            out += "(";
            out += std::to_string(i+1);
            out += ") = ";
            out += variable.as<std::vector<std::string>>()[i];
            out += "\n";
        }
        return out;
    }

    if (pwscf_character_var.find(variable.key) != pwscf_character_var.end()) {
        out += variable.key;
        out += " = \'";
        out += variable.as<std::string>();
        out += "\'\n";
        return out;
    }
    if (1 == variable.value.size()) {
        if (1 == variable.value[0].size()) {
            out += variable.key + " = " + variable.value[0][0];
        } else {
            out += variable.key + " =";
            for (const auto& item : variable.value[0]) {
                out += " " + item;
            }
        }
    } else {
        out += variable.key + " =";
        for (const auto& val : variable.value[0]) {
            out += " " + val;
        }
        out += "\n";
        for (int row = 1; row < variable.value.size() - 1; row++) {
            for (const auto& val : variable.value[row]) {
                out += " " + val;
            }
            out += "\n";
        }
        for (const auto& val : variable.value[variable.value.size() - 1]) {
            out += " " + val;
        }
    }
    return out;
}

} // namespace atomsciflow
