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

/// @file src/atomsciflow/nwchem/nwchem.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 03:28:12 PM CST

#include "atomsciflow/nwchem/nwchem.h"

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <iomanip>

namespace atomsciflow {

namespace fs = boost::filesystem;

NWChem::NWChem() {
    
    add_keyword("Charge", 0);
    add_keyword("title", "NWChem calculation");

    job.set_run_default("llhpc");
    job.set_run_default("pbs");
    job.set_run_default("bash");
    job.set_run_default("lsf_sz");
    job.set_run_default("lsf_sustc");
    job.set_run_default("cdcloud");

    job.set_run("cmd", "$ASF_CMD_NWCHEM");
    job.set_run("script_name_head", "nwchem-run");    
}

NWChem::~NWChem() {

}

// task directive must be specified in the end
std::string NWChem::to_string() {
    std::ostringstream out;
    for (const auto& item : this->directives) {
        if ("task" == item.first) {
            continue;
        }
        out << item.second->to_string() << "\n";
    }
    out << this->directives["task"]->to_string();
    return out.str();
}

void NWChem::new_directive(const std::string& name) {
    this->directives[name] = std::make_shared<nwchem::Directive>(name);
}

template <typename T>
void NWChem::add_keyword(const std::string& path, T keyword) {
    if (this->directives.find(path) == this->directives.end()) {
        this->new_directive(path);
    }
    this->directives[path]->keywords.push_back(boost::lexical_cast<std::string>(keyword));
}

/**
 * Sometimes in NWChem, only two keywords make sense, like "units angstroms"
 * Thus, we provide an overloaded template here.
 * Note that usually the keyword_1 in such case is usually a string, and 
 * when build the instantiation of the template we only sepcify std::string
 * for keyword_1 type.
 */ 
template <typename T, typename U>
void NWChem::add_keyword(const std::string& path, T keyword_1, U keyword_2) {
    this->add_keyword(path, keyword_1);
    this->add_keyword(path, keyword_2);
}

void NWChem::py_add_keyword(const std::string& path, int keyword) {
    this->add_keyword(path, keyword);
}

void NWChem::py_add_keyword(const std::string& path, double keyword) {
    this->add_keyword(path, keyword);
}

void NWChem::py_add_keyword(const std::string& path, std::string keyword) {
    this->add_keyword(path, keyword);
}

void NWChem::py_add_keyword(const std::string& path, std::string keyword_1, int keyword_2) {
    this->add_keyword(path, keyword_1);
    this->add_keyword(path, keyword_2);
}

void NWChem::py_add_keyword(const std::string& path, std::string keyword_1, double keyword_2) {
    this->add_keyword(path, keyword_1);
    this->add_keyword(path, keyword_2);
}

void NWChem::py_add_keyword(const std::string& path, std::string keyword_1, std::string keyword_2) {
    this->add_keyword(path, keyword_1);
    this->add_keyword(path, keyword_2);
}

void NWChem::py_set_field(const std::string& path, int field, int row, int col) {
    this->set_field(path, field, row, col);
}

void NWChem::py_set_field(const std::string& path, double field, int row, int col) {
    this->set_field(path, field, row, col);
}

void NWChem::py_set_field(const std::string& path, std::string field, int row, int col) {
    this->set_field(path, field, row, col);
}

void NWChem::get_xyz(const std::string& filepath) {
    this->xyz.read_xyz_file(filepath);
    job.set_run("xyz_file", fs::absolute(filepath).string());

    this->new_directive("geometry");
    this->directives["geometry"]->keywords.push_back("units");
    this->directives["geometry"]->keywords.push_back("angstroms");
    for (const auto& item : this->xyz.atoms) {
        std::vector<std::string> atom;
        atom.push_back(item.name);
        atom.push_back(boost::lexical_cast<std::string>(item.x));
        atom.push_back(boost::lexical_cast<std::string>(item.y));
        atom.push_back(boost::lexical_cast<std::string>(item.z)); 
        this->directives["geometry"]->fields.push_back(atom);
    }

    this->new_directive("basis");
    for (const auto& item : this->xyz.elements_set) {
        std::vector<std::string> basis;
        basis.push_back(item);
        basis.push_back("library");
        basis.push_back("6-31g");
        this->directives["basis"]->fields.push_back(basis);
    }
}

void NWChem::run(const std::string& directory) {    
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << "cat > nwchem.nw<<EOF\n";
    step << this->to_string();
    step << "EOF\n";
    step << "$CMD_HEAD " << job.run_params["cmd"] << "\n";
    job.steps.push_back(step.str());
    step.clear();

    job.run(directory);
}

// explicit template instantiation
template void NWChem::add_keyword<int>(const std::string&, int);
template void NWChem::add_keyword<double>(const std::string&, double);
template void NWChem::add_keyword<std::string>(const std::string&, std::string);

template void NWChem::add_keyword<std::string, int>(const std::string&, std::string, int);
template void NWChem::add_keyword<std::string, double>(const std::string&, std::string, double);
template void NWChem::add_keyword<std::string, std::string>(const std::string&, std::string, std::string);
} // namespace atomsciflow
