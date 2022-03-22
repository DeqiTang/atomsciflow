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

/// @file src/atomsciflow/variable/variable.h
/// @author deqi
/// Mail: deqi_tang@163.com
/// Created Time: Mon 25 Jan 2021 08:22:31 PM CST

#ifndef ATOMSCIFLOW_VARIABLE_VARIABLE_H_
#define ATOMSCIFLOW_VARIABLE_VARIABLE_H_

#include <string>
#include <vector>

#include "atomsciflow/variable/utils.h"

namespace atomsciflow {

class Variable {
public:
    Variable() {};
    ~Variable() {};

    Variable(std::string key, int value) {
        this->set(key, value);
    }

    Variable(std::string key, double value) {
        this->set(key, value);
    }

    Variable(std::string key, std::string value) {
        this->set(key, value);
    }

    Variable(std::string key, std::vector<int> value) {
        this->set(key, value);
    }

    Variable(std::string key, std::vector<double> value) {
        this->set(key, value);
    }

    Variable(std::string key, std::vector<std::string> value) {
        this->set(key, value);
    }

    Variable(std::string key, std::vector<std::vector<int> > value) {
        this->set(key, value);
    }

    Variable(std::string key, std::vector<std::vector<double> > value) {
        this->set(key, value);
    }

    Variable(std::string key, std::vector<std::vector<std::string> > value) {
        this->set(key, value);
    }

    void set(std::string key, int value);
    void set(std::string key, double value);
    void set(std::string key, std::string value);
    void set(std::string key, std::vector<int> value);
    void set(std::string key, std::vector<double> value);
    void set(std::string key, std::vector<std::string> value);
    void set(std::string key, std::vector<std::vector<int>> value);
    void set(std::string key, std::vector<std::vector<double>> value);
    void set(std::string key, std::vector<std::vector<std::string>> value);

    // with implicit instantiation
    template<typename U>
    U as() {
        U out;
        this->to(out);
        return out;
    }

    void to(int& value);
    void to(double& value);
    void to(std::string& value);
    void to(std::vector<int>& value);
    void to(std::vector<double>& value);
    void to(std::vector<std::string>& value);
    void to(std::vector<std::vector<int>>& value);
    void to(std::vector<std::vector<double>>& value);
    void to(std::vector<std::vector<std::string>>& value);

    void set_n(int n) { this->n = n; }

    std::string to_string();
    std::string to_string(int n);
    std::string to_string(std::string layout, std::string indent);
    std::string to_string(std::string layout, std::string indent, int n);

    std::string key;
    int n;
    std::vector<std::vector<std::string>> value;

    bool status = true;
private:
};

} // namespace atomsciflow
#endif // ATOMSCIFLOW_VARIABLE_VARIABLE_H_
