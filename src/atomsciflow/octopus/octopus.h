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

    template <typename T>
    void set_block_data(const std::string& name, std::vector<T> value, int i_row) {
        this->new_block(name);
        set_block_data_size(name, i_row + 1, value.size());
        for (int j = 0; j < value.size(); j++) {
            this->blocks[name]->data[i_row][j] = boost::lexical_cast<std::string>(value[j]);
        }
        for (int j = value.size(); j < this->blocks[name]->data[i_row].size(); j++) {
            this->blocks[name]->data[i_row][j] = "";
        }        
    }

    template <typename T>
    void set_block_data(const std::string& name, std::vector<std::vector<T>> value) {
        this->new_block(name);
        set_block_data_size(name, value.size(), value[0].size());
        for (int i = 0; i < value.size(); i++) {
            for (int j = 0; j < value[i].size(); j++) {
                this->blocks[name]->data[i][j] = boost::lexical_cast<std::string>(value[i][j]);
            }
        }
        for (int i = 0; i < value.size(); i++) {
            for (int j = value[i].size(); j < this->blocks[name]->data[i].size(); j++) {
                this->blocks[name]->data[i][j] = "";
            }
        }
        for (int i = value.size(); i < this->blocks[name]->data.size(); i++) {
            for (int j = 0; j < this->blocks[name]->data[i].size(); j++) {
                this->blocks[name]->data[i][j] = "";
            }
        }
    }

    void set_block_data_size(const std::string& name, int num_rows, int num_cols) {
        this->new_block(name);

        if (blocks[name]->data.size() < num_rows) {
            blocks[name]->data.resize(num_rows);
        }
        for (auto& item : blocks[name]->data) {
            if (item.size() < num_cols) {
                item.resize(num_cols);
            }
        }
        blocks[name]->num_rows = num_rows;
        blocks[name]->num_cols = num_cols;
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
    void py_set_block_data(const std::string& name, std::vector<int> value, int i_row) {
        this->set_block_data(name, value, i_row);
    }
    void py_set_block_data(const std::string& name, std::vector<double> value, int i_row) {
        this->set_block_data(name, value, i_row);
    }
    void py_set_block_data(const std::string& name, std::vector<std::string> value, int i_row) {
        this->set_block_data(name, value, i_row);
    }
    void py_set_block_data(const std::string& name, std::vector<std::vector<int>> value) {
        this->set_block_data(name, value);
    }
    void py_set_block_data(const std::string& name, std::vector<std::vector<double>> value) {
        this->set_block_data(name, value);
    }
    void py_set_block_data(const std::string& name, std::vector<std::vector<std::string>> value) {
        this->set_block_data(name, value);
    }

    void set_status(const std::string& key, bool status) {
        this->non_block->set_status(key, status);
    }
    
    void set_block_status(const std::string& name, bool status) {
        this->new_block(name);
        this->blocks[name]->status = status;
    }

    virtual void run(const std::string& directory);

    std::shared_ptr<VariableGroup> non_block;
    std::map<std::string, std::shared_ptr<octopus::Block>> blocks;
    Xyz xyz;
    JobScheduler job;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_OCTOPUS_OCTOPUS_H_
