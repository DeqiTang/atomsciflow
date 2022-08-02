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

/// @file src/atomsciflow/elk/block.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Fri 11 Mar 2022 04:07:48 PM CST

#include "atomsciflow/elk/block.h"

#include <sstream>
#include <boost/format.hpp>

namespace atomsciflow::elk {

std::string Block::to_string() {
    std::ostringstream out;
    
    out << this->name << "\n";

    for (const auto& item : this->params) {
        if (item.second.status == false) {
            continue;
        }

        if (0 == item.second.value.size()) {
            return item.second.key;
        }
        if (item.second.value.size() == 1) {
            if (item.second.value[0].size() == 1) {
                out << boost::format("  %1% : %2%\n\n") 
                    % item.second.value[0][0] % item.second.key;
            } else {
                for (const auto& item : item.second.value[0]) {
                    out << "  " << item;
                }
                out << " : " << item.second.key << "\n";
            }
        } else {
            int i = 0;
            for (const auto& row : item.second.value) {
                for (const auto& val : row) {
                    out << "  " << val;
                }
                if (i == 0) {
                    out << " : " << item.second.key;
                }
                i++;
                out << "\n";
            }
        }
    }
    out << "\n";

    return out.str();
}

} // namespace atomsciflow::elk
