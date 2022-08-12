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

/// @file src/atomsciflow/vasp/vasp_incar.cpp
/// @author DeqiTang
/// Mail deqitang@gmail.com
/// Created Time  Mon 07 Mar 2022 02 31 05 PM CST

#include "atomsciflow/vasp/vasp_incar.h"

#include "atomsciflow/variable/utils.h"
#include "atomsciflow/vasp/utils.h"

namespace atomsciflow {

VaspIncar::VaspIncar() {
    this->runtype = "static";
}

void VaspIncar::set_runtype(const std::string& runtype) {
    this->runtype = runtype;
    this->basic_setting();
}

std::string VaspIncar::to_string() {
    std::string out = "";

    std::string start = incar_group_title("start parameters");
    std::string electrons = incar_group_title("electrons parameters");
    std::string ions = incar_group_title("ions paramaters");
    std::string write = incar_group_title("write parameters");
    std::string dipolecorrection = incar_group_title("dipole correction");
    std::string xc = incar_group_title("exchange correlation");
    std::string lr = incar_group_title("linear response");
    std::string orbitalmagnet = incar_group_title("orbital magnet");
    std::string misc = incar_group_title("miscellaneous");
    std::string neb = incar_group_title("neb parameters");

    for (const auto& item : this->params) {
        if (false == this->params[item.first].status) {
            continue;
        }
        if (grouping.start_incharge.find(item.first) != grouping.start_incharge.end()) {
            start += vasp_variable_to_string(this->params[item.first]) + "\n";
        } else if (grouping.electrons_incharge.find(item.first) != grouping.electrons_incharge.end()) {
            electrons += vasp_variable_to_string(this->params[item.first]) + "\n";
        } else if (grouping.ions_incharge.find(item.first) != grouping.ions_incharge.end()) {
            ions += vasp_variable_to_string(this->params[item.first]) + "\n";    
        } else if (grouping.write_incharge.find(item.first) != grouping.write_incharge.end()) {
            write += vasp_variable_to_string(this->params[item.first]) + "\n";
        } else if (grouping.dipolecorrection_incharge.find(item.first) != grouping.dipolecorrection_incharge.end()) {
            dipolecorrection += vasp_variable_to_string(this->params[item.first]) + "\n";
        } else if (grouping.xc_incharge.find(item.first) != grouping.xc_incharge.end()) {
            xc += vasp_variable_to_string(this->params[item.first]) + "\n";
        } else if (grouping.lr_incharge.find(item.first) != grouping.lr_incharge.end()) {
            lr += vasp_variable_to_string(this->params[item.first]) + "\n";
        } else if (grouping.orbitalmagnet_incharge.find(item.first) != grouping.orbitalmagnet_incharge.end()) {
            orbitalmagnet += vasp_variable_to_string(this->params[item.first]) + "\n";
        } else if (grouping.neb_incharge.find(item.first) != grouping.neb_incharge.end()) {
            neb += vasp_variable_to_string(this->params[item.first]) + "\n";
        } else {
            misc += vasp_variable_to_string(this->params[item.first]) + "\n";
        }
    }
    std::set<std::string> tmp_set;
    tmp_set.insert("static");
    tmp_set.insert("opt");
    tmp_set.insert("md");
    tmp_set.insert("dfpt");
    tmp_set.insert("neb");
    tmp_set.insert("phonon");
    tmp_set.insert("custom");

    if (tmp_set.find(this->runtype) != tmp_set.end()) {
        out += start;
        out += electrons;
        out += ions;
    }
    out += xc;
    out += write;
    out += misc;

    tmp_set.clear();
    tmp_set.insert("dftp");
    tmp_set.insert("custom");
    if (tmp_set.find(this->runtype) != tmp_set.end()) {
        out += lr;
    }
    tmp_set.clear();
    tmp_set.insert("neb");
    tmp_set.insert("custom");
    if (tmp_set.find(this->runtype) != tmp_set.end()) {
        out += neb;
    }
    return out;
}   

void VaspIncar::basic_setting() {
    set_param("ENCUT",300);
    set_param("EDIFF", 1.0E-6);
    set_param("ISMEAR", 0);
    set_param("NWRITE", 2);

    if ("static" == this->runtype) {
        this->set_param("IBRION", -1);
    } else if ("opt" == this->runtype) {
        this->set_param("EDIFFG", 1.0E-3);
        this->set_param("IBRION", 1);
        this->set_param("ISIF", 0);
        this->set_param("NSW", 100);
    } else if ("md" == this->runtype) {
        this->set_status("EDIFFG", false);
        this->set_param("IBRION",0);
        // POTIM has different meaning in relaxation
        // and molecular dyanmics running.
        this->set_param("POTIM", 0.5);
        this->set_param("NSW", 1000);
    } else if ("phonon" == this->runtype) {
        this->set_param("IBRION", 7);
        this->set_param("NFREE", 4);
        this->set_param("POTIM", 0.015);
        this->set_param("NSW", 1); //
    } else if ("dfpt" == this->runtype) {
        this->set_param("LEPSILON", "T");
        this->set_param("LRPA", "T");
    } else if ("neb" == this->runtype) {
        // when doing neb calculation using VTST
        // must set EDIFFG < 0, when IOPT >0
        // set IBRION =3 and POTIM = 0  if
        // VTST optimizer is used
        this->set_param("ISIF", 2);
        this->set_param("EDIFFG", -0.01);
        this->set_param("IBRION", 3);
        this->set_param("POTIM", 0);
        this->set_param("LCLIMB", "T");
        this->set_param("IOPT", 1);
        this->set_param("SPRING", -5);
    } else if ("neb" == this->runtype) {

    }
}

/**
 * @brief VaspIncar::set_md
 * @param ensemble
 *  candidates;
 *      0 -> NVE
 *      1 -> NVT
 *      2 -> NPT
 *      3 -> NPH
 * @param thermostat
 *  candidates:
 *      0 -> Anderson
 *      1 -> Nose-Hoover
 *      2 -> Langevin
 *      3 -> Multiple Anderson
 */
void VaspIncar::set_md(int ensemble = 0, int thermostat = 0) {
    if (0 == ensemble) {
        this->set_param("MDALGO", 0);
        this->set_param("SMASS", -3);
    } else if (1 == ensemble) {
        this->set_param("ISIF", 2);
        if (0 == thermostat) {
            this->set_param("MDALGO", 1);
        } else if (thermostat == 1) {
            this->set_param("MDALGO", 2);
        } else if (thermostat == 2) {
            this->set_param("MDALGO", 3);
        } else if (thermostat == 3) {
            this->set_param("MDALGO", 13);
        }
    } else if (2 == ensemble) {
        this->set_param("MDALGO", 3);
        this->set_param("ISIF", 3);
    } else if (3 == ensemble) {
        this->set_param("MDALGO", 3);
        this->set_param("ISIF", 3);
        this->set_param("LANGEVIN_GAMMA_L", 0.0);
    }
}

} // namespace atomsciflow
