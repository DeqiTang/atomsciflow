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

/// @file src/atomsciflow/cp2k/section.h
/// @author deqi
/// Mail: deqi_tang@163.com
/// Created Time: Sun 31 Jan 2021 07:46:25 PM CST

#ifndef ATOMSCIFLOW_CP2K_SECTION_H_
#define ATOMSCIFLOW_CP2K_SECTION_H_

#include <map>

#include "atomsciflow/variable/variable.h"

namespace atomsciflow {

class Cp2kSection {
public:

    Cp2kSection() {
    };
    explicit Cp2kSection(const std::string& name) { 
        this->name = name; 
    }
    ~Cp2kSection() {};

    // Cp2kSection(const Cp2kSection& section) {
    //     *this = section;
    // }

    // Cp2kSection& operator=(const Cp2kSection& rhs) {
    //     this->name = rhs.name;
    //     this->section_parameter = rhs.section_parameter;
    //     this->section_var = rhs.section_var;
    //     this->status = rhs.status;
    //     this->sections = rhs.sections;
    //     this->params = rhs.params;  
    //     return *this;      
    // }

    std::string to_string();
    std::string to_string(std::string indent);

    Cp2kSection& add_section(const std::string& name);
    Cp2kSection& add_section(const std::string& name, Cp2kSection);
    void remove_section(std::string name);

    void set_param(std::string key, int value);
    void set_param(std::string key, double value);
    void set_param(std::string key, std::string value);
    void set_param(std::string key, std::vector<int> value);
    void set_param(std::string key, std::vector<double> value);
    void set_param(std::string key, std::vector<std::string> value);
    void set_param(std::string key, std::vector<std::vector<int>> value);
    void set_param(std::string key, std::vector<std::vector<double>> value);
    void set_param(std::string key, std::vector<std::vector<std::string>> value);

    bool contains(std::string key);
    void set_status(std::string key, bool status);
    void remove_param(std::string key);

    void clear();

    template<typename U>
    U get(std::string key);

    std::string name = "unknown";
    std::string section_parameter;
    Variable section_var;
    bool status = true;
    std::map<std::string, Cp2kSection> sections;

    private:
    std::map<std::string, Variable> params;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_CP2K_SECTION_H_
