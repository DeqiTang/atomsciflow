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

/// @file src/atomsciflow/abinit/abinit_dfpt.h
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Tue 08 Mar 2022 02:58:08 PM CST

#ifndef ATOMSCIFLOW_ABINIT_ABINIT_DFPT_H_
#define ATOMSCIFLOW_ABINIT_ABINIT_DFPT_H_

#include <set>

#include "atomsciflow/variable/group.h"

namespace atomsciflow {

// Reference:
// https://docs.abinit.org/guide/respfn/
// https://docs.abinit.org/tutorial/rf1/
// https://docs.abinit.org/tutorial/rf2/
// https://docs.abinit.org/tutorial/elastic/
// https://docs.abinit.org/tutorial/ffield/
// https://docs.abinit.org/tutorial/nlo/
// https://docs.abinit.org/topics/DFPT/
// https://docs.abinit.org/variables/dfpt/

class AbinitDfpt : public VariableGroup {
public:
    AbinitDfpt();
    ~AbinitDfpt();
    std::string to_string(int n);

    void basic_setting();

    std::set<std::string> incharge;
    bool status;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_ABINIT_ABINIT_DFPT_H_
