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

/// @file src/atomsciflow/abinit/abinit_files.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Tue 08 Mar 2022 03:10:56 PM CST

#include "atomsciflow/abinit/abinit_files.h"

#include <sstream>
#include <boost/format.hpp>

namespace atomsciflow {

std::string AbinitFiles::to_string(AbinitSystem& system) {
    std::ostringstream out;
    out << this->main_in << "\n";
    out << this->main_out << "\n";
    out << this->wavefunc_in << "\n";
    out << this->wavefunc_out << "\n";
    out << this->tmp << "\n";
    for (const auto& element : system.xyz.elements_set) {
        out << (element + ".psp8");
        out << "\n";
    }
    return out.str();
}

} // namespace atomsciflow
