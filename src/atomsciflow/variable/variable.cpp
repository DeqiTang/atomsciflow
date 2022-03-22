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

/// @file src/atomsciflow/varialbe/variable.cpp
/// @author deqi
/// Mail: deqi_tang@163.com
/// Created Time: Sat 30 Jan 2021 01:11:54 PM CST

#include "atomsciflow/variable/variable.h"

#include <string>

namespace atomsciflow {

using namespace utils;

inline std::string to_string_same_line(const Variable& var, std::string indent, int n) {
    if (false == var.status) {
        return "";
    }
    std::string out = "";
    if (0 == var.value.size()) {
        return out + var.key;
    }
    if (1 == var.value.size()) {
        if (1 == var.value[0].size()) {
            out += indent + var.key + n_to_string(n) + " " + var.value[0][0];
        } else {
            out += indent + var.key + n_to_string(n);
            for (const auto& item : var.value[0]) {
                out += " " + item;
            }
        }
    } else {
        out += indent + var.key + n_to_string(n);
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

inline std::string to_string_second_line(const Variable& var, std::string indent, int n) {
    if (false == var.status) {
        return "";
    }
    std::string out = "";
    if (0 == var.value.size()) {
        return out + var.key;
    }

    if (1 == var.value.size()) {
        if (1 == var.value[0].size()) {
            out += indent + var.key + n_to_string(n) + " " + var.value[0][0];
        } else {
            out += indent + var.key + n_to_string(n) + "\n";

            out += indent;
            for (const auto& item : var.value[0]) {
                out += " " + item;
            }
        }
    } else {
        out += indent + var.key + n_to_string(n) + "\n";
        for (const auto& row : var.value) {
            out += indent;
            for (const auto& val : row) {
                out += " " + val;
            }
            out += "\n";
        }
    }
    return out;
}

void Variable::set(std::string key, int value) {
    this->key = key;
    this->value.clear();
    this->value.push_back(std::vector<std::string>{std::to_string(value)});
}

void Variable::set(std::string key, double value) {
    this->key = key;
    this->value.clear();
    this->value.push_back(std::vector<std::string>{std::to_string(value)});
}

void Variable::set(std::string key, std::string value) {
    this->key = key;
    this->value.clear();
    this->value.push_back(std::vector<std::string>{value});
}

void Variable::set(std::string key, std::vector<int> value) {
    this->key = key;
    this->value.clear();
    std::vector<std::string> vec_str;
    for (const auto& i : value) {
        vec_str.push_back(std::to_string(i));
    }
    this->value.push_back(vec_str);
}

void Variable::set(std::string key, std::vector<double> value) {
    this->key = key;
    this->value.clear();
    std::vector<std::string> vec_str;
    for (const auto& i : value) {
        vec_str.push_back(std::to_string(i));
    }
    this->value.push_back(vec_str);
}

void Variable::set(std::string key, std::vector<std::string> value) {
    this->key = key;
    this->value.clear();
    std::vector<std::string> vec_str;
    for (const auto& val : value) {
        vec_str.push_back(val);
    }
    this->value.push_back(vec_str);
}

void Variable::set(std::string key, std::vector<std::vector<int> > value) {
    this->key = key;
    this->value.clear();
    std::vector<std::string> vec_str;
    for (const auto& row : value) {
        vec_str.clear();
        for (const auto& val : row) {
            vec_str.push_back(std::to_string(val));
        }
        this->value.push_back(vec_str);
    }
}

void Variable::set(std::string key, std::vector<std::vector<double> > value) {
    this->key = key;
    this->value.clear();
    std::vector<std::string> vec_str;
    for (const auto& row : value) {
        vec_str.clear();
        for (const auto& val : row) {
            vec_str.push_back(std::to_string(val));
        }
        this->value.push_back(vec_str);
    }
}

void Variable::set(std::string key, std::vector<std::vector<std::string> > value) {
    this->key = key;
    this->value.clear();
    std::vector<std::string> vec_str;
    for (const auto& row : value) {
        vec_str.clear();
        for (const auto& val : row) {
            vec_str.push_back(val);
        }
        this->value.push_back(vec_str);
    }
}

void Variable::to(int& value) {
    value = std::atoi(this->value[0][0].c_str());
}

void Variable::to(double& value) {
    value = std::atof(this->value[0][0].c_str());
}

void Variable::to(std::string& value) {
    value = this->value[0][0];
}

void Variable::to(std::vector<int>& value) {
    value.clear();
    for (auto& val : this->value[0]) {
        value.push_back(std::atoi(val.c_str()));
    }
}

void Variable::to(std::vector<double>& value) {
    value.clear();
    for (const auto& val : this->value[0]) {
        value.push_back(std::atof(val.c_str()));
    }
}

void Variable::to(std::vector<std::string>& value) {
    value.clear();
    //for (auto& val : this->value[0]) {
    //    value.push_back(val);
    //}
    value = this->value[0];
}

void Variable::to(std::vector<std::vector<int>>& value) {
    value.clear();
    std::vector<int> vec_int;
    for (const auto& row : this->value) {
        vec_int.clear();
        for (const auto& val : row) {
            vec_int.push_back(std::atoi(val.c_str()));
        }
        value.push_back(vec_int);
    }
}

void Variable::to(std::vector<std::vector<double>>& value) {
    value.clear();
    std::vector<double> vec_double;
    for (const auto& row : this->value) {
        vec_double.clear();
        for (const auto& val : row) {
            vec_double.push_back(std::atof(val.c_str()));
        }
        value.push_back(vec_double);
    }
}

void Variable::to(std::vector<std::vector<std::string>>& value) {
    value.clear();
    //std::vector<std::string> vec_str;
    //for (auto& row : this->value) {
    //    vec_double.clear();
    //    for (auto& val : row) {
    //        vec_double.push_back(val);
    //    }
    //    value.push_back(vec_str);
    //}
    value = this->value;
}

std::string Variable::to_string() {
    if (false == this->status) {
        return "";
    }
    return this->to_string(0);
}

std::string Variable::to_string(int n) {
    if (false == this->status) {
        return "";
    }
    std::string out = "";
    if (9 == this->value.size()) {
        return out + this->key;
    }
    if (this->value.size() == 1) {
        if (this->value[0].size() == 1) {
            out += this->key + n_to_string(n) + " " + this->value[0][0];
        } else {
            out += this->key + n_to_string(n) + "\n";
            for (const auto& item : this->value[0]) {
                out += " " + item;
            }
        }
    } else {
        out += this->key + n_to_string(n) + "\n";
        for (const auto& row : this->value) {
            for (const auto& val : row) {
                out += " " + val;
            }
            out += "\n";
        }
    }
    return out;
}

std::string Variable::to_string(std::string layout, std::string indent) {
    if (false == this->status) {
        return "";
    }
    return this->to_string(layout, indent, 0);
}

/// @param layout
///     "same-line"
///     "second-line"
std::string Variable::to_string(std::string layout, std::string indent, int n) {
    if (false == this->status) {
        return "";
    }
    if ("same-line" == layout) {
        return to_string_same_line(*this, indent, n);
    } else if ("second-line" == layout) {
        return to_string_second_line(*this, indent, n);
    } else {
        return to_string(n);
    }
}

} // namespace atomsciflow
