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

/// @file src/atomsciflow/abinit/abinit_input.h
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Tue 08 Mar 2022 02:36:32 PM CST

#ifndef ATOMSCIFLOW_ABINIT_ABINIT_INPUT_H_
#define ATOMSCIFLOW_ABINIT_ABINIT_INPUT_H_

#include <set>
#include <string>

#include "atomsciflow/abinit/abinit_electrons.h"
#include "atomsciflow/abinit/abinit_system.h"
#include "atomsciflow/abinit/abinit_ions.h"
#include "atomsciflow/abinit/abinit_dfpt.h"
#include "atomsciflow/abinit/abinit_files.h"
#include "atomsciflow/abinit/abinit_misc.h"

namespace atomsciflow {

class AbinitInput {
public:
    AbinitInput();
    ~AbinitInput();

    std::string to_string();
    void get_xyz(std::string xyzfile);
    
    template <typename T>
    void set_param(std::string key, T value) {
        if (this->electrons->incharge.find(key) != this->electrons->incharge.end()) {
            this->electrons->set_param(key, value);
            return;
        } else if (this->ions->incharge.find(key) != this->ions->incharge.end()) {
            this->ions->set_param(key, value);
            return;
        } else if (this->dfpt->incharge.find(key) != this->dfpt->incharge.end()) {
            this->dfpt->set_param(key, value);
            return;
        } else {
            this->misc->set_param(key, value);
        }
    }

    void set_params(std::map<std::string, std::string>& params);
    void set_n(int n);
    void set_kpoints(std::map<std::string, std::string>& kpoints);

    AbinitSystem* system;
    AbinitElectrons* electrons;
    AbinitIons* ions;
    AbinitDfpt* dfpt;
    AbinitMisc* misc;

    /// @var n int
    ///  n is used in generating the input string
    ///  for multi datasets mode Abinit calculation
    int n;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_ABINIT_ABINIT_INPUT_H_
