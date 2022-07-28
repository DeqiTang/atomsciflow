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

#include "atomsciflow/variable/group.h"
#include "atomsciflow/nwchem/directive.h"
#include "atomsciflow/base/xyz.h"
#include "atomsciflow/server/job_scheduler.h"

namespace atomsciflow {

class NWChem {
public:
    NWChem();
    ~NWChem();
    
    void get_xyz(const std::string& filepath);
    std::string to_string();
    
    void new_directive(const std::string& name);

    template <typename T>
    void add_keyword(const std::string& path, T keyword);

    template <typename T, typename U>
    void add_keyword(const std::string& path, T keyword_1, U keyword_2);

    void py_add_keyword(const std::string& path, int keyword);
    void py_add_keyword(const std::string& path, double keyword);
    void py_add_keyword(const std::string& path, std::string keyword);
    void py_add_keyword(const std::string& path, std::string keyword_1, int keyword_2);
    void py_add_keyword(const std::string& path, std::string keyword_1, double keyword_2);
    void py_add_keyword(const std::string& path, std::string keyword_1, std::string keyword_2);

    template <typename T>
    void set_field(const std::string& path, T field, int row, int col) {
        set_field_size(path, row, col);
        this->directives[path]->fields[row][col] = boost::lexical_cast<std::string>(field);
    }

    void set_field_size(const std::string& path, int row, int col) {
        if (this->directives[path]->fields.size() < row) {
            this->directives[path]->fields.resize(row);
        }
        for (auto& item : this->directives[path]->fields) {
            if (item.size() < col) {
                item.resize(col);
            }
        }
        // for (std::vector<std::vector<std::string>>::iterator it = this->directives[path]->fields.begin(); 
        //     it != this->directives[path]->fields.end(); 
        //     ++it) {
        //     if (it->size() < col) {
        //         it->resize(col);
        //     }
        // }
    }

    void py_set_field(const std::string& path, int field, int row, int col);
    void py_set_field(const std::string& path, double field, int row, int col);
    void py_set_field(const std::string& path, std::string field, int row, int col);

    template <typename T>
    void set_keywords(const std::string& path, const std::vector<T>& keywords) {
        if (this->directives.find(path) == this->directives.end()) {
            this->new_directive(path);
        }
        this->directives[path]->keywords.clear();
        for (const auto& item : keywords) {
            this->directives[path]->keywords.push_back(boost::lexical_cast<std::string>(item));
        }
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

    void set_simple(const std::string& name, bool simple);

    virtual void run(const std::string& directory);

    std::map<std::string, std::shared_ptr<nwchem::Directive>> directives;
    Xyz xyz;
    JobScheduler job;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_NWCHEM_NWCHEM_H_
