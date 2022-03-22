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

/// @file src/atomsciflow/variable/group_v2.h
/// @author deqi
/// Mail: deqi_tang@163.com
/// Created Time: Sat 30 Jan 2021 08:14:26 PM CST

#ifndef ATOMSCIFLOW_VARIABLE_GROUP_V2_H_
#define ATOMSCIFLOW_VARIABLE_GROUP_V2_H_

#include <map>
#include <iostream>
#include <iterator>
#include <typeinfo>

#include "atomsciflow/variable/variable_v2.h"

namespace atomsciflow {

class VariableGroupV2 {
public:
    VariableGroupV2() {};
    virtual ~VariableGroupV2() {};

    virtual void set_param(std::string key, int value);
    virtual void set_param(std::string key, double value);
    virtual void set_param(std::string key, std::vector<int> value);
    virtual void set_param(std::string key, std::vector<double> value);

    virtual std::string to_string();
    virtual std::string to_string(int n);

    virtual bool contains(std::string key);
    virtual void set_status(std::string key, bool status);
    virtual void remove(std::string key);

    virtual void clear();

    template<typename U>
    U get(std::string key);

    int n;
private:
    std::map<std::string, VariableV2<int> > params_int;
    std::map<std::string, VariableV2<double> > params_double;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_VARIABLE_GROUP_V2_H_
