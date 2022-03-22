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

/// @file src/atomsciflow/variable/variable_v2.cpp
/// @author Deqi Tang
/// Mail: deqi_tang@163.com
/// Created Time: Sat 30 Jan 2021 01:11:54 PM CST

#include "atomsciflow/variable/variable_v2.h"

#include <string>

namespace atomsciflow {
    
using namespace utils;

template<typename T>
void VariableV2<T>::set(std::string key, T value) {
    this->key = key;
    this->value.clear();
    this->value.push_back(std::vector<T>{value});
}

template<typename T>
void VariableV2<T>::set(std::string key, std::vector<T> value) {
    this->key = key;
    this->value.clear();
    this->value.push_back(value);
}

template<typename T>
void VariableV2<T>::set(std::string key, std::vector<std::vector<T> > value) {
    this->key = key;
    this->value = value;
}

/**
 * if we do not specify the object type explicitly
 * we can only export to calss template type T
 * and that's not enough
 */
template<typename T>
void VariableV2<T>::to(T& value) {
    value = this->value[0][0];
}

template<typename T>
void VariableV2<T>::to(std::vector<T>& value) {
    value = this->value[0];
}

template<typename T>
void VariableV2<T>::to(std::vector<std::vector<T>>& value) {
    value = this->value;
}

template<typename T>
std::string VariableV2<T>::to_string() {
    return this->to_string(this->n);
}

template<typename T>
std::string VariableV2<T>::to_string(int n) {
    std::string out = "";
    if (this->value.size() == 1) {
        if (this->value[0].size() == 1) {
            out += this->key + n_to_string(n) + " " + std::to_string(this->value[0][0]);
        } else {
            out += this->key + n_to_string(n);
            for (int i = 0; i < this->value[0].size(); i ++) {
                out += " " + std::to_string(this->value[0][i]);
            }
        }
    } else {
        out += this->key + n_to_string(n) + "\n";
        for (int i = 0; i < this->value.size(); i++) {
            for (int j = 0; j < this->value[i].size(); j++) {
                out += " " + std::to_string(this->value[i][j]);
            }
            out += "\n";
        }
    }
    return out;
}

// explicit template instantiation
template class VariableV2<int>;
template class VariableV2<double>;

} // namespace atomsciflow
