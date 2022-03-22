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

/// @file src/atomsciflow/qmcpack/qmcpack.h
/// @author DeqiTang
/// Mail: deqi_tang@163.com
/// Created Time: Wed 17 Nov 2021 10:18:15 AM CST

#ifndef ATOMSCIFLOW_QMCPACK_QMCPACK_H_
#define ATOMSCIFLOW_QMCPACK_QMCPACK_H_

#include "atomsciflow/qmcpack/input.h"

#include <map>

#include "atomsciflow/base/xyz.h"
#include "atomsciflow/server/job_scheduler.h"

namespace atomsciflow {

namespace pt = boost::property_tree;

class Qmcpack {

public:

    Qmcpack();
    
    ~Qmcpack() = default;

    void get_xyz(const std::string& xyzfile);
    std::string to_string();

    void set_job_steps_default();

    virtual void run(const std::string& directory);

    qmcpack::Input input;
    Xyz xyz;

    JobScheduler job;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_QMCPACK_QMCPACK_H_
