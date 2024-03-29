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

/// @file src/atomsciflow/abinit/abinit.h
/// @author Deqi Tang
/// Mail: deqi_tang@163.com
/// Created Time: Sun 31 Jan 2021 12:31:04 AM CST

#ifndef ATOMSCIFLOW_ABINIT_ABINIT_H_
#define ATOMSCIFLOW_ABINIT_ABINIT_H_

#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "atomsciflow/abinit/abinit_input.h"
#include "atomsciflow/abinit/abinit_files.h"
#include "atomsciflow/server/job_scheduler.h"
#include "atomsciflow/config/config_manager.h"

namespace atomsciflow {

namespace fs = boost::filesystem;
namespace ba = boost::algorithm;

class Abinit {
public:
    
    explicit Abinit();
    virtual ~Abinit();

    void get_xyz(const std::string& xyzfile);

    template <typename T>
    void set_param(std::string key, T value);
    
    void py_set_param(std::string key, int value);
    void py_set_param(std::string key, double value);
    void py_set_param(std::string key, std::string value);
    void py_set_param(std::string key, std::vector<int> value);
    void py_set_param(std::string key, std::vector<double> value);
    void py_set_param(std::string key, std::vector<std::string> value);
    void py_set_param(std::string key, std::vector<std::vector<int>> value);
    void py_set_param(std::string key, std::vector<std::vector<double>> value);
    void py_set_param(std::string key, std::vector<std::vector<std::string>> value);

    template <typename T>
    void set_params(std::map<std::string, T>& params);

    void py_set_params(std::map<std::string, int>& params);
    void py_set_params(std::map<std::string, double>& params);
    void py_set_params(std::map<std::string, std::string>& params);
    void py_set_params(std::map<std::string, std::vector<int>>& params);
    void py_set_params(std::map<std::string, std::vector<double>>& params);
    void py_set_params(std::map<std::string, std::vector<std::string>>& params);
    void py_set_params(std::map<std::string, std::vector<std::vector<int>>>& params);
    void py_set_params(std::map<std::string, std::vector<std::vector<double>>>& params);
    void py_set_params(std::map<std::string, std::vector<std::vector<std::string>>>& params);

    void set_kpoints(std::map<std::string, std::string>& kpoints, int ndtset);
    void set_pot(const std::string& pot);
    std::string to_string();
    void set_ndtset(int ndtset);

    void use_tol(std::string tol, double value, int dataset_i) {
        this->datasets[dataset_i]->use_tol(tol, value);
    }

    virtual void run(const std::string& directory);

    /// \brief ndtset is used to determine the number of datasets
    /// that are in effect.
    /// ndtset = 0 -> single dataset mode
    /// ndtset > 0 -> multi dataset mode
    int ndtset;

    std::vector<AbinitInput*> datasets;
    std::string pseudo_ext;
    JobScheduler job;
    ConfigManager config;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_ABINIT_ABINIT_H_
