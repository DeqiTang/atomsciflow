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

/// @file src/atomsciflow/cp2k/cp2k.h
/// @author deqi
/// Mail: deqi_tang@163.com
/// Created Time: Sun 31 Jan 2021 07:24:18 PM CST

#ifndef ATOMSCIFLOW_CP2K_CP2K_H_
#define ATOMSCIFLOW_CP2K_CP2K_H_

#include <string>
#include <memory>
//#include <format> // support for c++20 only

#include "atomsciflow/cp2k/section.h"
#include "atomsciflow/base/xyz.h"
#include "atomsciflow/server/job_scheduler.h"
#include "atomsciflow/plumed/plumed.h"

namespace atomsciflow {

class Cp2k {
public:

    Cp2k();
    ~Cp2k();

    void new_section(const std::string& path);
    bool exists_section(const std::string& path);
    
    template <typename T>
    void set_param(const std::string& path, T value);

    void py_set_param(const std::string& path, int value);
    void py_set_param(const std::string& path, double value);
    void py_set_param(const std::string& path, std::string value);
    void py_set_param(const std::string& path, std::vector<int> value);
    void py_set_param(const std::string& path, std::vector<double> value);
    void py_set_param(const std::string& path, std::vector<std::string> value);
    void py_set_param(const std::string& path, std::vector<std::vector<int>> value);
    void py_set_param(const std::string& path, std::vector<std::vector<double>> value);
    void py_set_param(const std::string& path, std::vector<std::vector<std::string>> value);

    std::string to_string();
    void get_xyz(const std::string& xyzfile);
    std::shared_ptr<Cp2kSection>& set_subsys(Xyz& xyz);
    void set_job_steps_default();

    virtual void run(const std::string& directory);

    std::map<std::string, std::shared_ptr<Cp2kSection>> sections;
    JobScheduler job;
    Xyz xyz;

    std::shared_ptr<plumed::Plumed> plumed;

private:
    std::shared_ptr<Cp2kSection>& set_subsys();
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_CP2K_CP2K_H_
