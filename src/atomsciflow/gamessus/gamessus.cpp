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

/// @file src/atomsciflow/gamessus/gamessus.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Fri 11 Mar 2022 09:48:47 AM CST

#include "atomsciflow/gamessus/gamessus.h"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <iomanip>

#include "atomsciflow/gamessus/utils.h"
#include "atomsciflow/base/element.h"
#include "atomsciflow/server/submit_script.h"
#include "atomsciflow/remote/server.h"

namespace atomsciflow {

namespace fs = boost::filesystem;

GamessUS::GamessUS() {

    job.set_run("cmd", "$ASF_CMD_GAMESSUS_RUNGMS");
    job.set_run("script_name_head", "gamessus-run");
}

GamessUS::~GamessUS() {
    for (const auto& item : this->groups) {
        delete item.second;
    }
}

std::string GamessUS::to_string() {
    std::ostringstream out;
    for (const auto& group : this->groups) {
        out << group_title(group.first);
        out << boost::format(" $%1%\n") % boost::to_upper_copy(group.first);
        for (const auto& item : group.second->params) {
            if (item.second.status == false) {
                continue;
            }
            if (0 == item.second.value.size()) {
                out << item.second.key;
            }
            if (item.second.value.size() == 1) {
                if (item.second.value[0].size() == 1) {
                    // out << item.second.key + " = " + item.second.value[0][0];
                    // no space should be left in the expression key=value,
                    // it might work for normal value with expression like key = value.
                    // but for logical value .TRUE. .FALSE. .true. .false. .T. .F.
                    // expression like key = .TRUE. will not work. and key=.TRUE. will work.
                    out << item.second.key + "=" + item.second.value[0][0];
                } else {
                    out << item.second.key + "\n";
                    for (const auto& item : item.second.value[0]) {
                        out << " " + item;
                    }
                }
            } else {
                out << item.second.key << "=" << "\n";
                for (const auto& row : item.second.value) {
                    for (const auto& val : row) {
                        out << " " + val;
                    }
                    out << "\n";
                }
            }
            out << "\n";
        }
        out << "\n";
        out << boost::format("$END\n");
    }

    out << "\n";
    auto element_map =  get_element_number_map();
    out << " $DATA\n";
    out << "geometry of the structure\n";
    // if group is C1, no blank is needed 
    // after the specification of symmetry group.
    out << "Cnv 2\n\n";
    // out << "C1\n"; 
    int i = 0;
    for (const auto& item : this->xyz.atoms) {
        out.setf(std::ios::fixed);
        out << item.name << " " << element_map[item.name].mass << " "
            << item.x << " " << item.y << " " << item.z << "\n";
        i += 1;
    }
    out << "$END\n";

    return out.str();
}

void GamessUS::get_xyz(const std::string& xyzfile) {
    this->xyz.read_xyz_file(xyzfile);
    job.set_run("xyz_file", fs::absolute(xyzfile).string());
}

void GamessUS::py_set_param(const std::string& path, int value) {
    this->set_param(path, value);
}

void GamessUS::py_set_param(const std::string& path, double value) {
    this->set_param(path, value);
}

void GamessUS::py_set_param(const std::string& path, std::string value) {
    this->set_param(path, value);
}

void GamessUS::py_set_param(const std::string& path, std::vector<int> value) {
    this->set_param(path, value);
}

void GamessUS::py_set_param(const std::string& path, std::vector<double> value) {
    this->set_param(path, value);
}

void GamessUS::py_set_param(const std::string& path, std::vector<std::string> value) {
    this->set_param(path, value);
}

void GamessUS::py_set_param(const std::string& path, std::vector<std::vector<int>> value) {
    this->set_param(path, value);
}

void GamessUS::py_set_param(const std::string& path, std::vector<std::vector<double>> value) {
    this->set_param(path, value);
}

void GamessUS::py_set_param(const std::string& path, std::vector<std::vector<std::string>> value) {
    this->set_param(path, value);
}

void GamessUS::run(const std::string& directory) {
    job.steps.clear();
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << "cat > gamessus.inp<<\'EOF\'\n";
    step << this->to_string();
    step << "EOF\n";
    step << boost::format("%1% gamessus.inp 00 12 >& gamessus.out\n") % job.run_params["cmd"];
    job.steps.push_back(step.str());
    step.clear();

    job.run(directory);
}

} // namespace atomsciflow
