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

/// @file src/atomsciflow/nwchem/nwchem.h
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 03:28:10 PM CST

#ifndef ATOMSCIFLOW_NWCHEM_NWCHEM_H_
#define ATOMSCIFLOW_NWCHEM_NWCHEM_H_

#include <memory>
#include <map>

#include "atomsciflow/variable/group.h"
#include "atomsciflow/nwchem/directive.h"
#include "atomsciflow/base/xyz.h"
#include "atomsciflow/server/job_scheduler.h"

namespace atomsciflow {

class NWChem {
public:
    NWChem();
    
    void get_xyz(const std::string& filepath);
    std::string to_string();
    
    void new_directive(const std::string& name);

    template <typename T>
    void set_param(const std::string& key, T value);

    void py_set_param(const std::string& key, int value);
    void py_set_param(const std::string& key, double value);
    void py_set_param(const std::string& key, std::string value);
    void py_set_param(const std::string& key, std::vector<int> value);
    void py_set_param(const std::string& key, std::vector<double> value);
    void py_set_param(const std::string& key, std::vector<std::string> value);
    void py_set_param(const std::string& key, std::vector<std::vector<int>> value);
    void py_set_param(const std::string& key, std::vector<std::vector<double>> value);
    void py_set_param(const std::string& key, std::vector<std::vector<std::string>> value);

    virtual void set_job_steps_default();
    virtual void run(const std::string& directory);

    std::shared_ptr<VariableGroup> non_directive;
    std::map<std::string, std::shared_ptr<nwchem::Directive>> directives;
    Xyz xyz;
    JobScheduler job;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_NWCHEM_NWCHEM_H_
