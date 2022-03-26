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

/// @file src/atomsciflow/qchem/qchem.h
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 03:39:58 PM CST

#ifndef ATOMSCIFLOW_QCHEM_QCHEM_H_
#define ATOMSCIFLOW_QCHEM_QCHEM_H_

#include <memory>
#include <map>

#include "atomsciflow/variable/group.h"
#include "atomsciflow/qchem/keyword_section.h"
#include "atomsciflow/base/xyz.h"
#include "atomsciflow/server/job_scheduler.h"

namespace atomsciflow {

class QChem {
public:

    QChem();
    ~QChem();

    std::string to_string();
    
    void new_section(const std::string& name);

    void get_xyz(const std::string& xyzfile);

    virtual void set_job_steps_default();
    virtual void run(const std::string& directory);

    std::shared_ptr<VariableGroup> non_block;
    std::map<std::string, std::shared_ptr<qchem::KeywordSection>> sections;
    Xyz xyz;
    JobScheduler job;
};

} //namespace atomsciflow

#endif // ATOMSCIFLOW_QCHEM_QCHEM_H_
