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

#include "atomsciflow/plumed/action.h"

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <sstream>

namespace atomsciflow::plumed {

namespace ba = boost::algorithm;

Action::Action() {
    this->label = "";
}

Action::~Action() {

}

std::string Action::to_string() {
    std::ostringstream out;
    if ("" != this->label) {
        out << boost::format("%1%: %2%") 
            % this->label
            % ba::to_upper_copy(this->name);
    } else {
        out << boost::format("%1%") % ba::to_upper_copy(this->name);
    }
    out << " ...\n";
    
    for (auto& item : this->params) {
        if (item.second.status == false) {
            continue;
        }

        if (0 == item.second.value.size()) {
            out << item.second.key;
        }
        if (item.second.value.size() == 1) {
            if (item.second.value[0].size() == 1) {
                out << boost::format("  %1%=%2%") 
                    % ba::to_upper_copy(item.second.key)
                    % item.second.value[0][0];
            } else {
                out << boost::format("  %1%=") % ba::to_upper_copy(item.second.key);
                int i = 0;
                for (const auto& item : item.second.value[0]) {
                    if (i == 0) {
                        out << item;
                    } else {
                        out << "," << item;
                    }
                    i++;
                }
            }
        } else {
            continue;
        }
        out << "\n";
    }

    out << "...\n";
    return out.str();
}

} // namespace atomsciflow::plumed
