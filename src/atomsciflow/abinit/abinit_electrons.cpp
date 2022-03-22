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

/// @file src/atomsciflow/abinit/abinit_electrons.cpp
/// @author Deqi Tang
/// Mail: deqi_tang@163.com
/// Created Time: Sat 30 Jan 2021 02:37:52 PM CST

#include "atomsciflow/abinit/abinit_electrons.h"

#include "atomsciflow/abinit/utils.h"

namespace atomsciflow {

std::string AbinitElectrons::to_string(int n = 0) {
    std::string out = "";
    out += group_setting_title("electrons related setting");
    out += "\n";
    this->set_n(n);
    out += VariableGroup::to_string(this->n);
    out += "\n";
    return out;
}

/**
 * @brief AbinitElectrons::use_tol
 * @param tol
 *  candidates:
 *      "toldfe" | "tolwfr" | "toldff" | "tolrff" | "tolvrs"
 * @param value
 * The specification of convergence tolerance in Abinit calcualtion
 * is miscellaneous and in multi ways. Thus we provide a interface
 * to manipulate it more convenienetly.
 */
void AbinitElectrons::use_tol(std::string tol, double value) {
    std::vector<std::string> tols{
        "toldfe",
        "tolwfr",
        "toldff",
        "tolrff",
        "tolvrs"
    };
    for (const auto& item : tols) {
        if (item != tol && this->contains(item)) {
            this->set_status(item, false);
        }
    }
    this->set_param(tol, value);
    this->set_status(tol, true);
}

/// \brief AbinitElectrons::set_scf_nscf
/// \param mode
/// Providing easy way to switch the scf and nscf calculation
/// mode in Abinit.
void AbinitElectrons::set_scf_nscf(std::string mode="scf") {
    if ("scf" == mode) {
        if (this->contains("usepaw") && this->get<int>("usepaw") == 1) {
            this->set_param("iscf", 17);
        } else {
            this->set_param("iscf", 7);
        }
        this->set_param("prtden", 1);
    }
    if ("nscf" == mode) {
        this->set_param("iscf", -3);
        this->set_param("nstep", 0);
    }
}

/// \brief AbinitElectrons::basic_setting
/// Some basic setting of the AbinitElectrons input group
/// for Abinit calculation.
void AbinitElectrons::basic_setting() {
    this->set_param("ecut", 15);
    this->set_param("occopt", 3);
    this->set_param("nstep", 100);
    this->set_param("diemac", 2.0);
    this->set_param("ixc", 11);
    this->use_tol("tolvrs", 1.0e-18);
}

} // namespace atomscifloww
