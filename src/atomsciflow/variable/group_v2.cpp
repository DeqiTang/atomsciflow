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

/// @file src/atomsciflow/variable/group_v2.cpp
/// @author deqi
/// Mail: deqi_tang@163.com
/// Created Time: Sat 30 Jan 2021 08:29:12 PM CST

#include "atomsciflow/variable/group_v2.h"

namespace atomsciflow {

using namespace utils;

void VariableGroupV2::set_param(std::string key, int value) {
    this->remove(key);
    this->params_int[key] = VariableV2<int>{key, value};
}

void VariableGroupV2::set_param(std::string key, double value) {

    this->remove(key);
    this->params_double[key] = VariableV2<double>{key, value};
}

void VariableGroupV2::set_param(std::string key, std::vector<int> value) {
    this->remove(key);

    this->params_int[key] = VariableV2<int>{key, value};
}

void VariableGroupV2::set_param(std::string key, std::vector<double> value) {
    this->remove(key);

    this->params_double[key] = VariableV2<double>{key, value};
}

    bool VariableGroupV2::contains(std::string key) {
    if (this->params_int.find(key) == this->params_int.end() && this->params_double.find(key) == this->params_double.end()) {
        return false;
    } else {
        return true;
    }
}

void VariableGroupV2::set_status(std::string key, bool status) {
    //this->params[key].status = status;
    if (this->contains(key) == false) {
        return;
    } else {
        if (this->params_int.find(key) == this->params_int.end()) {
            this->params_double[key].status = status;
        } else {
            this->params_int[key].status = status;
        }
    }
}

std::string VariableGroupV2::to_string() {
    return this->to_string(this->n);
}

std::string VariableGroupV2::to_string(int n) {
    std::string out = "";
    for (const auto& item : this->params_int) {
        if (this->params_int[item.first].status == false) {
            continue;
        }
        out += this->params_int[item.first].to_string(n) + "\n";
    }
    for (const auto& item : this->params_double) {
        if (this->params_double[item.first].status == false) {
            continue;
        }
        out += this->params_double[item.first].to_string(n) + "\n";
    }
    return out;
}

void VariableGroupV2::remove(std::string key) {
    for (auto it = this->params_int.begin(); it != this->params_int.end(); ++it) {
        if (it->first == key) {
            this->params_int.erase(it);
            //return;
        }
    }
    for (auto it = this->params_double.begin(); it != this->params_double.end(); ++it) {
        if (it->first == key) {
            this->params_double.erase(it);
            //return;
        }
    }
}

void VariableGroupV2::clear() {
    this->params_int.clear();
    this->params_double.clear();
}

template<typename U>
U VariableGroupV2::get(std::string key) {
    U out{};
    for (auto it = this->params_int.begin(); it != this->params_int.end(); ++it) {
        if (it->first == key) {
            //this->params_int[key].to(out);
            out = this->params_int[key].as<U>();
            //return out;
        }
    }
    /*
    for (auto it = this->params_double.begin(); it != this->params_double.end(); ++it) {
        if (it->first == key) {
            this->params_double[key].to(out);
            this->params_double[key].as<U>();
            //return out;
        }   
    }
    */
    return out;
}

// explicit template instantiation
template int VariableGroupV2::get(std::string);
//template double VariableGroupV2::get(std::string);
template std::vector<int> VariableGroupV2::get(std::string);

} // namespace atomsciflow
