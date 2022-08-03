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

/// @file src/atomsciflow/octopus/octopus.h
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 02:51:51 PM CST

#ifndef ATOMSCIFLOW_OCTOPUS_OCTOPUS_H_
#define ATOMSCIFLOW_OCTOPUS_OCTOPUS_H_

#include <memory>
#include <map>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "atomsciflow/variable/group.h"
#include "atomsciflow/octopus/block.h"
#include "atomsciflow/base/xyz.h"
#include "atomsciflow/server/job_scheduler.h"

namespace atomsciflow {

class Octopus {
public:

    Octopus();
    ~Octopus();

    std::string to_string();

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

    void new_block(const std::string& name);

    void get_xyz(const std::string& xyzfile);

    template <typename T>
    void set_block_data(const std::string& name, T value, int i_row, int i_col) {
        this->new_block(name);
        set_block_data_size(name, i_row + 1, i_col + 1);
        this->blocks[name]->data[i_row][i_col] = boost::lexical_cast<std::string>(value);
    }

    void set_block_data_size(const std::string& name, int num_row, int num_col) {
        this->new_block(name);

        if (blocks[name]->data.size() < num_row) {
            blocks[name]->data.resize(num_row);
        }
        for (auto& item : blocks[name]->data) {
            if (item.size() < num_col) {
                item.resize(num_col);
            }
        }
    }

    void py_set_block_data(const std::string& name, int value, int row, int col) {
        this->set_block_data(name, value, row, col);
    }
    void py_set_block_data(const std::string& name, double value, int row, int col) {
        this->set_block_data(name, value, row, col);
    }
    void py_set_block_data(const std::string& name, std::string value, int row, int col) {
        this->set_block_data(name, value, row, col);
    }

    virtual void run(const std::string& directory);

    std::shared_ptr<VariableGroup> non_block;
    std::map<std::string, std::shared_ptr<octopus::Block>> blocks;
    Xyz xyz;
    JobScheduler job;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_OCTOPUS_OCTOPUS_H_
