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

#ifndef ATOMSCIFLOW_CP2K_UTILS_H_
#define ATOMSCIFLOW_CP2K_UTILS_H_

#include <boost/algorithm/string.hpp>

#include "atomsciflow/variable/variable.h"

namespace atomsciflow::cp2k {

namespace ba = boost::algorithm;

inline std::string to_string_same_line(const Variable& var, std::string indent) {
    if (false == var.status) {
        return "";
    }
    std::string out = "";
    // var.key may contain array information, so remove it.
    std::vector<std::string> vec_str;
    ba::split(vec_str, var.key, boost::is_any_of("["));
    std::string var_key_inp = vec_str[0];
    
    if (0 == var.value.size()) {
        return out + var_key_inp;
    }
    if (1 == var.value.size()) {
        if (1 == var.value[0].size()) {
            out += indent + var_key_inp + " " + var.value[0][0];
        } else {
            out += indent + var_key_inp;
            for (const auto& item : var.value[0]) {
                out += " " + item;
            }
        }
    } else {
        out += indent + var_key_inp;
        for (const auto& val : var.value[0]) {
            out += " " + val;
        }
        out += "\n";
        for (int row = 1; row < var.value.size() - 1; ++row) {
            out += indent;
            for (const auto& val : var.value[row]) {
                out += " " + val;
            }
            out += "\n";
        }
        out += indent;
        for (const auto& val : var.value[var.value.size()-1]) {
            out += " " + val;
        }
    }
    return out;
}

} // name space atomsciflow::cp2k

#endif // ATOMSCIFLOW_CP2K_UTILS_H_