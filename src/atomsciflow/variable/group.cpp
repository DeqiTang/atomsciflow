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

/// @file group.cpp
/// @author deqi
/// Mail: deqi_tang@163.com
/// Created Time: Sat 30 Jan 2021 08:29:12 PM CST

#include "atomsciflow/variable/group.h"

namespace atomsciflow {

using namespace utils;

void VariableGroup::set_param(std::string key, int value) {
    this->remove(key);
    this->params[key] = Variable{key, value};
}

void VariableGroup::set_param(std::string key, double value) {
    this->remove(key);
    this->params[key] = Variable{key, value};
}

void VariableGroup::set_param(std::string key, std::string value) {
    this->remove(key);
    this->params[key] = Variable{key, value};
}

void VariableGroup::set_param(std::string key, std::vector<int> value) {
    this->remove(key);
    this->params[key] = Variable{key, value};
}

void VariableGroup::set_param(std::string key, std::vector<double> value) {
    this->remove(key);
    this->params[key] = Variable{key, value};
}

void VariableGroup::set_param(std::string key, std::vector<std::string> value) {
    this->remove(key);
    this->params[key] = Variable{key, value};
}

void VariableGroup::set_param(std::string key, std::vector<std::vector<int>> value) {
    this->remove(key);
    this->params[key] = Variable{key, value};
}

void VariableGroup::set_param(std::string key, std::vector<std::vector<double>> value) {
    this->remove(key);
    this->params[key] = Variable{key, value};
}

void VariableGroup::set_param(std::string key, std::vector<std::vector<std::string>> value) {
    this->remove(key);
    this->params[key] = Variable{key, value};
}

bool VariableGroup::contains(std::string key) {
    if (this->params.find(key) == this->params.end()) {
        return false;
    } else {
        return true;
    }
}

void VariableGroup::set_status(std::string key, bool status) {
    if (this->contains(key) == false) {
        return;
    }
    this->params[key].status = status;
}

void VariableGroup::set_n(int n) {
    this->n = n;
}

std::string VariableGroup::to_string() {
    return this->to_string(this->n);
}

std::string VariableGroup::to_string(int n) {
    std::string out = "";
    for (const auto& item : this->params) {
        if (this->params[item.first].status == false) {
            continue;
        }
        out += this->params[item.first].to_string(n) + "\n";
    }
    return out;
}

void VariableGroup::remove(std::string key) {
    for (auto it = this->params.begin(); it != this->params.end(); ++it) {
        if (it->first == key) {
            this->params.erase(it);
            return;
        }
    }
}

void VariableGroup::clear() {
    this->params.clear();
}

template<typename U>
U VariableGroup::get(std::string key) {
    U out;
    //this->params[key].to(out);
    out = this->params[key].as<U>();
    return out;
}

// explicit template instantiation
template int VariableGroup::get<int>(std::string);
template double VariableGroup::get<double>(std::string);
template std::string VariableGroup::get<std::string>(std::string);
template std::vector<int> VariableGroup::get<std::vector<int>>(std::string);
template std::vector<double> VariableGroup::get<std::vector<double>>(std::string);
template std::vector<std::string> VariableGroup::get<std::vector<std::string>>(std::string);
template std::vector<std::vector<int>> VariableGroup::get<std::vector<std::vector<int>>>(std::string);
template std::vector<std::vector<double>> VariableGroup::get<std::vector<std::vector<double>>>(std::string);
template std::vector<std::vector<std::string>> VariableGroup::get<std::vector<std::vector<std::string>>>(std::string);

} // namespace atomsciflow
