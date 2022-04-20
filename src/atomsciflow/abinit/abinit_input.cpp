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

/// @file src/atomsciflow/abinit/abinit_input.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Tue 08 Mar 2022 02:37:42 PM CST

#include "atomsciflow/abinit/abinit_input.h"

namespace atomsciflow {

AbinitInput::AbinitInput() {
    this->system = new AbinitSystem{};
    this->electrons = new AbinitElectrons{};
    this->ions = new AbinitIons{};
    this->dfpt = new AbinitDfpt{};
    this->misc = new AbinitMisc{};

    this->n = 0;
}

AbinitInput::~AbinitInput() {
    delete this->system;
    delete this->electrons;
    delete this->ions;
    delete this->dfpt;
    delete this->misc;
}

std::string AbinitInput::to_string() {
    std::string out = "";
    if (true == this->system->status) {
        out += this->system->to_string(this->n);
    }
    if (true == this->electrons->status) {
        out += this->electrons->to_string(this->n);
    }
    if (true == this->ions->status) {
        out += this->ions->to_string(this->n);
    }
    if (true == this->dfpt->status) {
        out += this->dfpt->to_string(this->n);
    }
    if (true == this->misc->status) {
        out += this->misc->to_string(this->n);
    }
    return out;
}

void AbinitInput::get_xyz(std::string xyzfile) {
    this->system->xyz.read_xyz_file(xyzfile);
}

void AbinitInput::set_params(std::map<std::string, std::string>& params) {
    for (const auto& item : params) {
        this->set_param(item.first, item.second);
    }
}

void AbinitInput::set_kpoints(std::map<std::string, std::string>& kpoints) {
    for (const auto& item : kpoints) {
        if (this->electrons->kpoints.incharge.find(item.first) != this->electrons->kpoints.incharge.end()) {
            this->electrons->kpoints.set_param(item.first, item.second);
        }
    }
}

void AbinitInput::set_n(int n) {
    this->n = n;
}

} // namespace atomsciflow
