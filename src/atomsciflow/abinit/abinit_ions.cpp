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

/// @file src/atomsciflow/abinit/abinit_ions.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Tue 08 Mar 2022 03:10:30 PM CST

#include "atomsciflow/abinit/abinit_ions.h"

#include "atomsciflow/abinit/utils.h"

namespace atomsciflow {

AbinitIons::AbinitIons() {
    incharge.insert("ionmov");
    incharge.insert("optcell");
    incharge.insert("ecutsm");
    incharge.insert("ntime");
    incharge.insert("tolmxde");
    incharge.insert("tolmxf");
    incharge.insert("dilatmx");
    status = true;
    this->basic_setting("opt");
}

std::string AbinitIons::to_string(int n = 0) {
    std::string out = "";
    out += group_setting_title("ions moving related setting");
    out += "\n";
    this->set_n(n);
    out += VariableGroup::to_string(this->n);
    out += "\n";
    return out;
}

/// \brief AbinitIons::basic_setting
/// \param mode
///     candidates:
///         "opt" | "md"
/// Some basic setting of the AbinitIons input group for Abinit
/// calculation.
void AbinitIons::basic_setting(const std::string& mode = "opt") {
    if ("opt" == mode) {
        this->set_param("ionmov", 3);
        this->set_param("optcell", 0);
        this->set_param("ntime", 100);
        this->set_param("tolmxf", 0);
        this->set_param("tolmxde", "0.0001 eV");
    } else if ("md" == mode) {
        this->set_param("ionmov", 8);
        this->set_param("dtion", 100);
        this->set_param("ntime", 1000);
        this->set_param("nctime", 1);
        this->set_param("mdtemp(1)", 300);
        this->set_param("mdtemp(2)", 300);
        this->set_param("tolmxf", 5.0e-4);
    }
}

} // namespace atomsciflow
