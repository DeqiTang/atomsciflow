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
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "atomsciflow/variable/group.h"
#include "atomsciflow/nwchem/directive.h"
#include "atomsciflow/base/xyz.h"
#include "atomsciflow/server/job_scheduler.h"

namespace atomsciflow::nwchem {

class NWChem {
public:
    NWChem();
    ~NWChem();
    
    void get_xyz(const std::string& filepath);
    std::string to_string();
    
    void new_directive(const std::string& path);

    template <typename T>
    void add_keyword(const std::string& path, T keyword);

    template <typename T, typename U>
    void add_keyword(const std::string& path, T keyword_1, U keyword_2);

    void py_add_keyword(const std::string& path, int keyword);
    void py_add_keyword(const std::string& path, double keyword);
    void py_add_keyword(const std::string& path, std::string keyword);
    void py_add_keyword(const std::string& path, int keyword_1, int keyword_2);
    void py_add_keyword(const std::string& path, double keyword_1, double keyword_2);
    void py_add_keyword(const std::string& path, std::string keyword_1, std::string keyword_2);

    /**
     * @brief Set the field object
     *
     * e.g.
     *  set_field("geometry[0]", "C", 0, 0);
     *  set_field("geometry@xxx[0]", "H", 1, 0); 
     * @tparam T 
     * @param path 
     * @param field 
     * @param i_row 
     * @param i_col 
     */
    template <typename T>
    void set_field(const std::string& path, T field, int i_row, int i_col) {
        this->new_directive(path);
        set_field_size(path, i_row + 1, i_col + 1);
        
        std::vector<std::string> str_vec_1;
        std::vector<std::string> str_vec_2;
        boost::split(str_vec_1, path, boost::is_any_of("["), boost::token_compress_on);
        boost::split(str_vec_2, str_vec_1[1], boost::is_any_of("]"), boost::token_compress_on);
        int i_task = boost::lexical_cast<int>(str_vec_2[0]);

        boost::split(str_vec_2, path, boost::is_any_of("/"), boost::token_compress_on);
        str_vec_2[0] = str_vec_1[0];
        int length = str_vec_2.size();

        auto field_recursive = [&] (auto&& _field_recursive, std::map<std::string, std::shared_ptr<Directive>>& _direcs, int i) {
            std::string direc_key = "";
            direc_key = str_vec_2[i];

            if (i + 1  == length) {
                _direcs[direc_key]->fields[i_row][i_col] = boost::lexical_cast<std::string>(field);
                return;
            }
            _field_recursive(_field_recursive, _direcs[direc_key]->directives, i + 1);
        };
        field_recursive(field_recursive, this->tasks[i_task], 0);
    }

    /**
     * @brief Set the field size object
     * e.g.
     *  set_field_size("geometry[0]", 10, 4);
     *  set_field_size("geometry@xxx[0]", 10, 4);
     * @param path 
     * @param num_row 
     * @param num_col 
     */
    void set_field_size(const std::string& path, int num_row, int num_col) {
        this->new_directive(path);

        std::vector<std::string> str_vec_1;
        std::vector<std::string> str_vec_2;
        boost::split(str_vec_1, path, boost::is_any_of("["), boost::token_compress_on);
        boost::split(str_vec_2, str_vec_1[1], boost::is_any_of("]"), boost::token_compress_on);
        int i_task = boost::lexical_cast<int>(str_vec_2[0]);

        boost::split(str_vec_2, path, boost::is_any_of("/"), boost::token_compress_on);
        str_vec_2[0] = str_vec_1[0];
        int length = str_vec_2.size();

        auto field_size_recursive = [&] (auto&& _field_size_recursive, std::map<std::string, std::shared_ptr<Directive>>& _direcs, int i) {
            std::string direc_key = "";
            direc_key = str_vec_2[i];

            if (i + 1  == length) {
                if (_direcs[direc_key]->fields.size() < num_row) {
                    _direcs[direc_key]->fields.resize(num_row);
                }
                for (auto& item : _direcs[direc_key]->fields) {
                    if (item.size() < num_col) {
                        item.resize(num_col);
                    }
                }
                return;
            }
            _field_size_recursive(_field_size_recursive, _direcs[direc_key]->directives, i + 1);
        };
        field_size_recursive(field_size_recursive, this->tasks[i_task], 0);
    }

    void py_set_field(const std::string& path, int field, int row, int col);
    void py_set_field(const std::string& path, double field, int row, int col);
    void py_set_field(const std::string& path, std::string field, int row, int col);

    /**
     * @brief Set the keywords object
     * e.g.
     *  set_keywords("task[0]", std::vector<std::string>{"dft", "energy"});
     *  set_keywords("geometry[0]/symmetry", std::vector<std::string>{"print"});
     *  set_keywords("geometry@xxx[0]/symmetry", std::vector<std::string>{"print"});
     * @tparam T 
     * @param path 
     * @param keywords 
     */
    template <typename T>
    void set_keywords(const std::string& path, const std::vector<T>& keywords) {
        this->new_directive(path);

        std::vector<std::string> str_vec_1;
        std::vector<std::string> str_vec_2;
        boost::split(str_vec_1, path, boost::is_any_of("["), boost::token_compress_on);
        boost::split(str_vec_2, str_vec_1[1], boost::is_any_of("]"), boost::token_compress_on);
        int i_task = boost::lexical_cast<int>(str_vec_2[0]);

        boost::split(str_vec_2, path, boost::is_any_of("/"), boost::token_compress_on);
        str_vec_2[0] = str_vec_1[0];
        int length = str_vec_2.size();

        auto keywords_recursive = [&] (auto&& _keywords_recursive, std::map<std::string, std::shared_ptr<Directive>>& _direcs, int i) {
            std::string direc_key = "";
            direc_key = str_vec_2[i];

            if (i + 1  == length) {
                _direcs[direc_key]->keywords.clear();
                for (const auto& item : keywords) {
                    _direcs[direc_key]->keywords.push_back(boost::lexical_cast<std::string>(item));
                }
                return;
            }
            _keywords_recursive(_keywords_recursive, _direcs[direc_key]->directives, i + 1);
        };
        keywords_recursive(keywords_recursive, this->tasks[i_task], 0);
    }

    void py_set_keywords(const std::string& path, const std::vector<int>& keywords) {
        this->set_keywords(path, keywords);
    }
    void py_set_keywords(const std::string& path, const std::vector<double>& keywords) {
        this->set_keywords(path, keywords);
    }
    void py_set_keywords(const std::string& path, const std::vector<std::string>& keywords) {
        this->set_keywords(path, keywords);
    }

    void set_simple(const std::string& path, bool simple);

    void set_simple_default();

    void set_status(const std::string& path, bool status);
    
    virtual void run(const std::string& directory);

    std::vector<std::map<std::string, std::shared_ptr<nwchem::Directive>>> tasks;
    Xyz xyz;
    JobScheduler job;
};

} // namespace atomsciflow::nwchem

#endif // ATOMSCIFLOW_NWCHEM_NWCHEM_H_
