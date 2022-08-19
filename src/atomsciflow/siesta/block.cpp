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

/// @file src/atomsciflow/siesta/block.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 04:04:45 PM CST

#include "atomsciflow/siesta/block.h"

#include <sstream>
#include <boost/format.hpp>

namespace atomsciflow::siesta {

Block::Block() {
    this->status = true;
}

Block::Block(const std::string& name) {
    this->name = name;
    this->status = true;
}

std::string Block::to_string() {
    if (this->status == false) {
        return "";
    }
    std::ostringstream out;
    out << "%block " << this->name << "\n";
    for (auto& row : this->data) {
        for (auto& val : row) {
            out << boost::format(" %1%") % val;
        }
        out << "\n";
    }    
    out << "%endblock " << this->name << "\n";
    return out.str();
}

} // namespace atomsciflow::siesta
