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

/// @file src/atomsciflow/gamessus/gamessus.h
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Fri 11 Mar 2022 09:48:17 AM CST

#ifndef ATOMSCIFLOW_GAMESSUS_GAMESSUS_H_
#define ATOMSCIFLOW_GAMESSUS_GAMESSUS_H_

#include <map>
#include <string>
#include <boost/algorithm/string.hpp>

#include "atomsciflow/variable/group.h"
#include "atomsciflow/base/xyz.h"
#include "atomsciflow/server/job_scheduler.h"

namespace atomsciflow {

class GamessUS {
public:

    GamessUS();
    ~GamessUS();

    std::string to_string();
    void get_xyz(const std::string& xyzfile);

    template<typename T>
    void set_param(const std::string& path, T value) {
        std::vector<std::string> str_vec;
        boost::split(str_vec, path, boost::is_any_of("/"), boost::token_compress_on);

        if (this->groups.find(str_vec[0]) == this->groups.end()) {
            this->groups[str_vec[0]] = new VariableGroup{};
        }
        this->groups[str_vec[0]]->set_param(str_vec[1], value);
    }

    void py_set_param(const std::string& path, int value);
    void py_set_param(const std::string& path, double value);
    void py_set_param(const std::string& path, std::string value);
    void py_set_param(const std::string& path, std::vector<int> value);
    void py_set_param(const std::string& path, std::vector<double> value);
    void py_set_param(const std::string& path, std::vector<std::string> value);
    void py_set_param(const std::string& path, std::vector<std::vector<int>> value);
    void py_set_param(const std::string& path, std::vector<std::vector<double>> value);
    void py_set_param(const std::string& path, std::vector<std::vector<std::string>> value);

    virtual void run(const std::string& directory);

    std::map<std::string, VariableGroup*> groups;
    Xyz xyz;
    JobScheduler job;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_GAMESSUS_GAMESSUS_H_
