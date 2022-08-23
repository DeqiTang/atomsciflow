/************************************************************************
MIT License

Copyright (c) 2022 Deqi Tang

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

#include "atomsciflow/qe/card.h"

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

namespace atomsciflow::qe {

Card::Card() {
    this->status = true;
    this->option = "";
}

Card::Card(const std::string& name) {
    this->name = name;
    this->status = true;
    this->option = "";
}

Card::~Card() {

}

std::string Card::to_string() {
    if (this->status == false) {
        return "";
    }
    std::ostringstream out;
    out << boost::format("%1% %2%\n") % boost::to_upper_copy(this->name) % this->option;

    for (auto& row : this->data) {
        for (auto& val : row) {
            out << boost::format(" %1%") % val;
        }
        out << "\n";
    }
    out << "\n";
    return out.str();
}

} // namespace atomsciflow:;qe
