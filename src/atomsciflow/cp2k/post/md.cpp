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

#include "atomsciflow/cp2k/post/md.h"

#include <regex>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

namespace atomsciflow::cp2k::post {

namespace fs = boost::filesystem;

MD::MD() {
    this->set_run("md-out", "cp2k.out");
    this->set_run("output-json", "post-md.json");

    this->add_rule("time-start-end", std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat1{"STARTED\\ AT|ENDED\\ AT"};
        std::regex pat2{"\\d{4}[-]\\d{2}[-]\\d{2}\\ [0-2][0-3]:[0-5][0-9]:[0-5][0-9]\\.[0-9]{3}"};
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, pat1)) {
            std::regex_search(str, m2, pat2);
            info.put(m1.str(0), m2.str(0));
        }
    }});

    // this->add_rule_type_1("time", "STARTED\\ AT|ENDED\\ AT", "\\d{4}[-]\\d{2}[-]\\d{2}\\ [0-2][0-3]:[0-5][0-9]:[0-5][0-9]\\.[0-9]{3}");

    pt::ptree energy_child;
    info.add_child("ENERGY| Total", energy_child);
    this->add_rule("total-energies", std::function<void(const std::string&)>{[&](const std::string& str) {
        std::regex pat("ENERGY\\| Total");
        std::regex energy_pat("[-][0-9]+\\.\\d+");
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, pat)) {
            std::regex_search(str, m2, energy_pat);
            info.get_child(m1.str(0)).push_back(pt::ptree::value_type("", m2.str(0)));
        }
    }});

    pt::ptree converge_child;
    info.add_child("SCF run converged in", converge_child);
    this->add_rule("scf-convergences", std::function<void(const std::string&)>{[&](const std::string& str) {
        std::regex pat("SCF run converged in");
        std::regex converge_pat("\\d+");
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, pat)) {
            std::regex_search(str, m2, converge_pat);
            info.get_child(m1.str(0)).push_back(pt::ptree::value_type("", m2.str(0)));
        }
    }});

    pt::ptree instantaneous_temperature_child;
    info.add_child("Instantaneous Temperature", instantaneous_temperature_child);
    pt::ptree average_temperature_child;
    info.add_child("Average Temperature", average_temperature_child);
    this->add_rule("temperatures", std::function<void(const std::string&)>{[&](const std::string& str) {
        std::regex pat("MD\\|\\ Temperature \\[K\\]\\ +\\d+\\.\\d+\\ +\\d+\\.\\d+");
        std::regex temp_pat("\\ \\d+\\.\\d+");
        std::smatch m1;
        std::smatch m2;
        std::vector<std::string> vec_str;
        if (std::regex_search(str, m1, pat)) {
            boost::split(vec_str, str, boost::is_any_of(" "), boost::token_compress_on);
            info.get_child("Instantaneous Temperature").push_back(pt::ptree::value_type("", vec_str[4]));
            info.get_child("Average Temperature").push_back(pt::ptree::value_type("", vec_str[5]));
        }
    }});

}

} // namespace atomsciflow::cp2k::post
