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
#include <boost/algorithm/string.hpp>

#include "atomsciflow/qe/gen_namelist.h"
#include "atomsciflow/qe/card.h"
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

    void new_card(const std::string& name) {
        if (this->cards.find(name) == this->cards.end()) {
            this->cards[name] = std::make_shared<Card>(name);
        }
    }

    template <typename T>
    void set_card_data(const std::string& name_option, T value, int i_row, int i_col) {
        std::string name = name_option;
        if (boost::contains(name_option, "(")) {
            std::vector<std::string> str_vec;
            boost::split(str_vec, name_option, boost::is_any_of("("), boost::token_compress_on);
            name = str_vec[0];
            this->set_card_option(name, boost::replace_all_copy(str_vec[1], ")", ""));
        }

        this->new_card(name);
        set_card_data_size(name, i_row + 1, i_col + 1);
        this->cards[name]->data[i_row][i_col] = boost::lexical_cast<std::string>(value);
    }

    template <typename T>
    void set_card_data(const std::string& name_option, std::vector<T> value, int i_row) {
        std::string name = name_option;
        if (boost::contains(name_option, "(")) {
            std::vector<std::string> str_vec;
            boost::split(str_vec, name_option, boost::is_any_of("("), boost::token_compress_on);
            name = str_vec[0];
            this->set_card_option(name, boost::replace_all_copy(str_vec[1], ")", ""));
        }
        
        this->new_card(name);
        set_card_data_size(name, i_row + 1, value.size());
        for (int j = 0; j < value.size(); j++) {
            this->cards[name]->data[i_row][j] = boost::lexical_cast<std::string>(value[j]);
        }
        for (int j = value.size(); j < this->cards[name]->data[i_row].size(); j++) {
            this->cards[name]->data[i_row][j] = "";
        }
    }

    template <typename T>
    void set_card_data(const std::string& name_option, std::vector<std::vector<T>> value) {
        std::string name = name_option;
        if (boost::contains(name_option, "(")) {
            std::vector<std::string> str_vec;
            boost::split(str_vec, name_option, boost::is_any_of("("), boost::token_compress_on);
            name = str_vec[0];
            this->set_card_option(name, boost::replace_all_copy(str_vec[1], ")", ""));
        }

        this->new_card(name);
        set_card_data_size(name, value.size(), value[0].size());
        for (int i = 0; i < value.size(); i++) {
            for (int j = 0; j < value[i].size(); j++) {
                this->cards[name]->data[i][j] = boost::lexical_cast<std::string>(value[i][j]);
            }
        }
        for (int i = 0; i < value.size(); i++) {
            for (int j = value[i].size(); j < this->cards[name]->data[i].size(); j++) {
                this->cards[name]->data[i][j] = "";
            }
        }
        for (int i = value.size(); i < this->cards[name]->data.size(); i++) {
            for (int j = 0; j < this->cards[name]->data[i].size(); j++) {
                this->cards[name]->data[i][j] = "";
            }
        }        
    }

    void set_card_data_size(const std::string& name_option, int num_row, int num_col) {
        std::string name = name_option;
        if (boost::contains(name_option, "(")) {
            std::vector<std::string> str_vec;
            boost::split(str_vec, name_option, boost::is_any_of("("), boost::token_compress_on);
            name = str_vec[0];
            this->set_card_option(name, boost::replace_all_copy(str_vec[1], ")", ""));
        }
        
        this->new_card(name);
        if (cards[name]->data.size() < num_row) {
            cards[name]->data.resize(num_row);
        }
        for (auto& item : cards[name]->data) {
            if (item.size() < num_col) {
                item.resize(num_col);
            }
        }
    }

    void py_set_card_data(const std::string& name, int value, int row, int col) {
        this->set_card_data(name, value, row, col);
    }
    void py_set_card_data(const std::string& name, double value, int row, int col) {
        this->set_card_data(name, value, row, col);
    }
    void py_set_card_data(const std::string& name, std::string value, int row, int col) {
        this->set_card_data(name, value, row, col);
    }
    void py_set_card_data(const std::string& name, std::vector<int> value, int i_row) {
        this->set_card_data(name, value, i_row);
    }
    void py_set_card_data(const std::string& name, std::vector<double> value, int i_row) {
        this->set_card_data(name, value, i_row);
    }
    void py_set_card_data(const std::string& name, std::vector<std::string> value, int i_row) {
        this->set_card_data(name, value, i_row);
    }
    void py_set_card_data(const std::string& name, std::vector<std::vector<int>> value) {
        this->set_card_data(name, value);
    }
    void py_set_card_data(const std::string& name, std::vector<std::vector<double>> value) {
        this->set_card_data(name, value);
    }
    void py_set_card_data(const std::string& name, std::vector<std::vector<std::string>> value) {
        this->set_card_data(name, value);
    }
    
    void set_card_status(const std::string& name, bool status) {
        this->new_card(name);
        this->cards[name]->status = status;
    }

    void set_card_option(const std::string& name, std::string option) {
        this->new_card(name);
        this->cards[name]->option = option;
    }

    void basic_setting_ions(std::string calc);
    void basic_setting_electrons();
    void basic_setting_control(std::string calc);
    void set_occupations(std::string occupations, std::string smearing, double degauss);

    virtual void run(const std::string& directory);

    std::map<std::string, Namelist> namelists;
    std::map<std::string, std::shared_ptr<Card>> cards;
    Xyz xyz;
    JobScheduler job;
    ConfigManager config;
};

} // namespace atomsciflow::qe

#endif // ATOMSCIFLOW_QE_PWSCF_H_
