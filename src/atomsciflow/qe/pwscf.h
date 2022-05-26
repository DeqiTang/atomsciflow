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

/// @file src/atomsciflow/qe/pwscf.h
/// @author deqi
/// Mail: deqi_tang@163.com
/// Created Time: Tue 02 Feb 2021 05:49:31 PM CST

#ifndef ATOMSCIFLOW_QE_PWSCF_H_
#define ATOMSCIFLOW_QE_PWSCF_H_

#include <map>

#include "atomsciflow/qe/gen_namelist.h"
#include "atomsciflow/base/xyz.h"
#include "atomsciflow/qe/misc.h"
#include "atomsciflow/server/job_scheduler.h"
#include "atomsciflow/config/config_manager.h"

namespace atomsciflow::qe {

class PwScf {
public:
    PwScf();
    ~PwScf();

    std::string to_string();
    std::string to_string(std::string indent);
    void get_xyz(const std::string& xyzfile);

    void set_param(const std::string& namelist, std::string key, int value);
    void set_param(const std::string& namelist, std::string key, double value);
    void set_param(const std::string& namelist, std::string key, std::string value);
    void set_param(const std::string& namelist, std::string key, std::vector<int> value);
    void set_param(const std::string& namelist, std::string key, std::vector<double> value);
    void set_param(const std::string& namelist, std::string key, std::vector<std::string> value);
    void set_param(const std::string& namelist, std::string key, std::vector<std::vector<int>> value);
    void set_param(const std::string& namelist, std::string key, std::vector<std::vector<double>> value);
    void set_param(const std::string& namelist, std::string key, std::vector<std::vector<std::string>> value);

    void set_kpoints(std::string kpoints_option, std::vector<int>& kpoints_mp, Kpath crystal_b);
    void basic_setting_ions(std::string calc);
    void basic_setting_electrons();
    void basic_setting_control(std::string calc);
    void basic_setting_system();
    void set_occupations(std::string occupations, std::string smearing, double degauss);

    virtual void run(const std::string& directory);

    std::map<std::string, Namelist> namelists;
    PwScfMisc misc;
    JobScheduler job;
    ConfigManager config;
};

} // namespace atomsciflow::qe

#endif // ATOMSCIFLOW_QE_PWSCF_H_
