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

/// @file src/atomsciflow/vasp/incar_grouping.h
/// @author: DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Sun 20 Mar 2022 02:26:50 PM CST

#ifndef ATOMSCIFLOW_VASP_INCAR_GROUPING_H_
#define ATOMSCIFLOW_VASP_INCAR_GROUPING_H_

#include <string>
#include <set>

namespace atomsciflow {

class IncarGrouping {
public:

    IncarGrouping();

    void init();

    std::set<std::string> start_incharge;
    std::set<std::string> xc_incharge;
    std::set<std::string> write_incharge;
    std::set<std::string>  orbitalmagnet_incharge;
    std::set<std::string> lr_incharge;
    std::set<std::string> ions_incharge;
    std::set<std::string> intraband_incharge;
    std::set<std::string> electrons_incharge;
    std::set<std::string> dipolecorrection_incharge;
    std::set<std::string> neb_incharge;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_VASP_INCAR_GROUPING_H_
