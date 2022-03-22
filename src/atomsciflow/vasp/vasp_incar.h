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

/// @file src/atomsciflow/vasp/vasp_incar.h
/// @author: DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Mon 07 Mar 2022 02:26:07 PM CST

#ifndef ATOMSCIFLOW_VASP_VASP_INCAR_H_
#define ATOMSCIFLOW_VASP_VASP_INCAR_H_

#include <set>
#include <string>

#include "atomsciflow/variable/group.h"
#include "atomsciflow/vasp/incar_grouping.h"

namespace atomsciflow {

std::string vasp_variable_to_string(atomsciflow::Variable& variable);

class VaspIncar: public VariableGroup {
public:
    VaspIncar();

    void basic_setting();
    void set_runtype(const std::string& runtype);
    void set_md();

    std::string to_string();

    void set_md(int ensemble, int thermostat);

    std::string runtype;
    IncarGrouping grouping;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_VASP_VASP_INCAR_H_
