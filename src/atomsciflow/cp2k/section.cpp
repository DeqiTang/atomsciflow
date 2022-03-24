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

/// @file src/atomsciflow/cp2k/section.cpp
/// @author deqi
/// Mail: deqi_tang@163.com
/// Created Time: Sun 31 Jan 2021 07:57:17 PM CST

#include "atomsciflow/cp2k/section.h"

#include <boost/algorithm/string.hpp>

namespace atomsciflow {

namespace ba = boost::algorithm;

std::string Cp2kSection::to_string() {
    if (this->status == false) {
        return "";
    }
    // this->name may contain array in formation, so remove it.
    std::vector<std::string> vec_str;
    ba::split(vec_str, this->name, boost::is_any_of("["));
    std::string name_inp = vec_str[0];
    
    std::string out = "";
    out += "&" + name_inp + " " + this->section_parameter + "\n";
    out += this->section_var.to_string() + "\n";
    for (const auto& item : this->params) {
        out += this->params[item.first].to_string() + "\n";
    }
    for (const auto& item : this->sections) {
        out += "\n";
        out += this->sections[item.first].to_string();
        out += "\n";
    }
    out += "&end " + name_inp;
    return out;
}

std::string Cp2kSection::to_string(std::string indent) {
    if (false == this->status) {
        return "";
    }
    // this->name may contain array in formation, so remove it.
    std::vector<std::string> vec_str;
    ba::split(vec_str, this->name, boost::is_any_of("["));
    std::string name_inp = vec_str[0];

    std::string out = "";
    out += indent + "&" + name_inp + " " + this->section_parameter + "\n";
    out += this->section_var.to_string("same-line", indent + indent) + "\n";

    for (const auto& item : this->params) {
        out += indent + indent + this->params[item.first].to_string("same-line", "") + "\n";
    }
    for (const auto& item : this->sections) {
        //out += "\n";
        out += this->sections[item.first].to_string(indent + indent);
        out += "\n";
    }
    out += indent + "&end " + name_inp;
    return out;
}

Cp2kSection& Cp2kSection::add_section(const std::string& name) {
    this->sections[name] = Cp2kSection{name};
    this->sections[name].name = name;
    return this->sections[name];
}

Cp2kSection& Cp2kSection::add_section(const std::string& name, Cp2kSection section) {
    this->sections[name] = section;
    this->sections[name].name = name;
    return this->sections[name];
}

void Cp2kSection::remove_section(std::string name) {
    for (auto it = this->sections.begin(); it != this->sections.end(); ++it) {
        if (it->first == name) {
            this->sections.erase(it);
        }
    }
}

void Cp2kSection::set_param(std::string key, int value) {
    //this->remove(key);
    this->params[key] = Variable{key, value};
}

void Cp2kSection::set_param(std::string key, double value) {
    //this->remove(key);
    this->params[key] = Variable{key, value};
}

void Cp2kSection::set_param(std::string key, std::string value) {
    //this->remove(key);
    this->params[key] = Variable{key, value};
}

void Cp2kSection::set_param(std::string key, std::vector<int> value) {
    //this->remove(key);
    this->params[key] = Variable{key, value};
}

void Cp2kSection::set_param(std::string key, std::vector<double> value) {
    //this->remove(key);
    this->params[key] = Variable{key, value};
}

void Cp2kSection::set_param(std::string key, std::vector<std::string> value) {
    //this->remove(key);
    this->params[key] = Variable{key, value};
}

void Cp2kSection::set_param(std::string key, std::vector<std::vector<int>> value) {
    //this->remove(key);
    this->params[key] = Variable{key, value};
}

void Cp2kSection::set_param(std::string key, std::vector<std::vector<double>> value) {
    //this->remove(key);
    this->params[key] = Variable{key, value};
}

void Cp2kSection::set_param(std::string key, std::vector<std::vector<std::string>> value) {
    //this->remove(key);
    this->params[key] = Variable{key, value};
}

bool Cp2kSection::contains(std::string key) {
    if (this->params.find(key) == this->params.end()) {
        return false;
    } else {
        return true;
    }
}

void Cp2kSection::set_status(std::string key, bool status) {
    if (this->contains(key) == false) {
        return;
    }
    this->params[key].status = status;
}

void Cp2kSection::remove_param(std::string key) {
    for (auto it = this->params.begin(); it != this->params.end(); ++it) {
        if (it->first == key) {
            this->params.erase(it);
            //return;
        }
    }
}

void Cp2kSection::clear() {
    this->params.clear();
}

template<typename U>
U Cp2kSection::get(std::string key) {
    U out;
    out = this->params[key].as<U>();
    return out;
}

// explicit instantiation of template member function
template int Cp2kSection::get<int>(std::string);
template double Cp2kSection::get<double>(std::string);
template std::string Cp2kSection::get<std::string>(std::string);
template std::vector<int> Cp2kSection::get<std::vector<int>>(std::string);
template std::vector<double> Cp2kSection::get<std::vector<double>>(std::string);
template std::vector<std::string> Cp2kSection::get<std::vector<std::string>>(std::string);
template std::vector<std::vector<int>> Cp2kSection::get<std::vector<std::vector<int>>>(std::string);
template std::vector<std::vector<double>> Cp2kSection::get<std::vector<std::vector<double>>>(std::string);
template std::vector<std::vector<std::string>> Cp2kSection::get<std::vector<std::vector<std::string>>>(std::string);

} //namespace atomsciflow
