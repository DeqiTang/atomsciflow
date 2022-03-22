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

/// @file src/atomsciflow/qmcpack/input.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Sun 13 Mar 2022 03:26:17 PM CST

#include "atomsciflow/qmcpack/input.h"

#include <sstream>

namespace atomsciflow::qmcpack {

Input::Input() {

}

Input::~Input() {

}

std::string Input::to_string() {
    std::string out = "";
    std::ostringstream os;
    pt::write_xml(os,
        this->root,
        pt::xml_writer_make_settings<std::string>(' ', 2)
    );
    out += os.str();
    out += "\n";
    return out;
}

} // namespace atomsciflow::qmcpack
