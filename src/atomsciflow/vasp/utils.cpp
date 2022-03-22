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

/// @file src/atomsciflow/vasp/utils.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Thu 17 Mar 2022 10:40:03 AM CST

#include "atomsciflow/vasp/utils.h"

namespace atomsciflow {

std::string vasp_variable_to_string(Variable& variable) {
    std::string out;

    if (false == variable.status) {
        return "";
    }
    out = "";
    if (0 == variable.value.size()) {
        return out + variable.key;
    }
    if (1 == variable.value.size()) {
        if (1 == variable.value[0].size()) {
            out += variable.key + " = " + variable.value[0][0];
        } else {
            out += variable.key + " =";
            for (const auto& item : variable.value[0]) {
                out += " " + item;
            }
        }
    } else {
        out += variable.key + " =";
        for (const auto& val : variable.value[0]) {
            out += " " + val;
        }
        out += "\n";
        for (int row = 1; row < variable.value.size() -1; row++) {
            for (const auto& val : variable.value[row]) {
                out += " " + val;
            }
            out += "\n";
        }
        for (const auto& val : variable.value[variable.value.size() - 1]) {
            out += " " + val;
        }
    }
    return out;
}

std::string incar_group_title(const std::string& title) {
    std::string out = "#----------------------------------------------------------------------#\n";
    int n_vacancies_left = 0;
    int n_vacancies_right = 0;
    n_vacancies_left = (72 - title.size() - 2) / 2;
    n_vacancies_right = 72 - 2 - title.size() - n_vacancies_left;
    out += "#";
    while (n_vacancies_left > 0) {
        out += " ";
        n_vacancies_left -= 1;
    }
    out += title;
    while (n_vacancies_right > 0) {
        out += " ";
        n_vacancies_right -= 1;
    }
    out += "#\n";
    out += "#----------------------------------------------------------------------#\n";

    return out;
}

} // namespace atomsciflow
