/************************************************************************
MIT License

Copyright (c) 2022 Deqi Tang

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

#ifndef ATOMSCIFLOW_PLUMED_PLUMED_H_
#define ATOMSCIFLOW_PLUMED_PLUMED_H_

#include <map>
#include <memory>
#include <boost/algorithm/string.hpp>

#include "atomsciflow/plumed/action.h"

namespace atomsciflow::plumed {

class Plumed {
public:

    Plumed();
    ~Plumed();

    std::string to_string();

    void add_action(std::string label_name);

    template <typename T>
    void set_param(const std::string& path, T value) {
        std::vector<std::string> str_vec_1;
        std::vector<std::string> str_vec_2;
        boost::split(str_vec_1, path, boost::is_any_of("/"), boost::token_compress_on);
        boost::split(str_vec_2, str_vec_1[0], boost::is_any_of(":"), boost::token_compress_on);
        std::string key = boost::join(str_vec_2, ":");
        this->add_action(key);
        this->actions[key]->set_param(str_vec_1[1], value);
    }

    void py_set_param(const std::string& path, int value) {
        this->set_param(path, value);
    }

    void py_set_param(const std::string& path, double value) {
        this->set_param(path, value);
    }

    void py_set_param(const std::string& path, std::string value) {
        this->set_param(path, value);
    }

    void py_set_param(const std::string& path, std::vector<int> value) {
        this->set_param(path, value);
    }

    void py_set_param(const std::string& path, std::vector<double> value) {
        this->set_param(path, value);
    }

    void py_set_param(const std::string& path, std::vector<std::string> value) {
        this->set_param(path, value);
    }

    /**
     * map of string to Action,
     * the string is in the following format:
     *  label:name
     *       :name
     *        name
     */
    std::map<std::string, std::shared_ptr<Action>> actions;
    std::vector<std::string> actions_order;
};

} // namespace atomsciflow::plumed

#endif // ATOMSCIFLOW_PLUMED_PLUMED_H_
