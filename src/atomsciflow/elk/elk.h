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

/// @file src/atomsciflow/elk/elk.h
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Wed 02 Mar 2022 02:38:11 PM CST

#ifndef ATOMSCIFLOW_ELK_ELK_H_
#define ATOMSCIFLOW_ELK_ELK_H_

#include <map>

#include "atomsciflow/elk/block.h"
#include "atomsciflow/base/xyz.h"
#include "atomsciflow/server/job_scheduler.h"

namespace atomsciflow {

class Elk {
public:

    Elk();
    ~Elk();

    void new_block(const std::string& name);
    std::string to_string();
    void get_xyz(std::string xyzfile);

    template <typename T>
    void set_param(const std::string& group, std::string key, T value);

    void py_set_param(const std::string& group, std::string key, int value);
    void py_set_param(const std::string& group, std::string key, double value);
    void py_set_param(const std::string& group, std::string key, std::string value);
    void py_set_param(const std::string& group, std::string key, std::vector<int> value);
    void py_set_param(const std::string& group, std::string key, std::vector<double> value);
    void py_set_param(const std::string& group, std::string key, std::vector<std::string> value);
    void py_set_param(const std::string& group, std::string key, std::vector<std::vector<int>> value);
    void py_set_param(const std::string& group, std::string key, std::vector<std::vector<double>> value);
    void py_set_param(const std::string& group, std::string key, std::vector<std::vector<std::string>> value);

    virtual void run(const std::string& directory);

    std::map<std::string, Block*> blocks;
    Xyz xyz;
    JobScheduler job;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_ELK_ELK_H_
