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

/// @file group.h
/// @author deqi
/// Mail: deqi_tang@163.com
/// Created Time: Sat 30 Jan 2021 08:14:26 PM CST

#ifndef ATOMSCIFLOW_VARIABLE_GROUP_H_
#define ATOMSCIFLOW_VARIABLE_GROUP_H_

#include <map>
#include <iostream>
#include <iterator>
#include <typeinfo>

//#include "atomsciflow/variable/utils.h"
#include "atomsciflow/variable/variable.h"

namespace atomsciflow {

class VariableGroup {
public:
    VariableGroup() {};
    virtual ~VariableGroup() {};

    virtual void set_param(std::string key, int value);
    virtual void set_param(std::string key, double value);
    virtual void set_param(std::string key, std::string value);
    virtual void set_param(std::string key, std::vector<int> value);
    virtual void set_param(std::string key, std::vector<double> value);
    virtual void set_param(std::string key, std::vector<std::string> value);
    virtual void set_param(std::string key, std::vector<std::vector<int>> value);
    virtual void set_param(std::string key, std::vector<std::vector<double>> value);
    virtual void set_param(std::string key, std::vector<std::vector<std::string>> value);

    virtual std::string to_string();
    virtual std::string to_string(int n);

    virtual bool contains(std::string key);
    virtual void set_status(std::string key, bool status);
    virtual void set_n(int n);
    virtual void remove(std::string key);

    virtual void clear();

    template<typename U>
    U get(std::string key);

    int n;
    std::map<std::string, Variable> params;

private:

};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_VARIABLE_GROUP_H_
