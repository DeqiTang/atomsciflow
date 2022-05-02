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

#include "atomsciflow/cp2k/post/opt.h"

#include <regex>
#include <fstream>
#include <boost/filesystem.hpp>

#include "atomsciflow/cp2k/post/utils.h"

namespace atomsciflow::cp2k::post {

namespace fs = boost::filesystem;

Opt::Opt() {
    this->set_run("opt-out", "cp2k.out");
    this->set_run("output-json", "post-opt.json");

    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat("STARTED\\ AT|ENDED\\ AT");
        std::regex time_pat("\\d{4}[-]\\d{2}[-]\\d{2}\\ [0-2][0-3]:[0-5][0-9]:[0-5][0-9]\\.[0-9]{3}");
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, pat)) {
            std::regex_search(str, m2, time_pat);
            info.put(m1.str(0), m2.str(0));
        }
    }});

    pt::ptree energy_child;
    info.add_child("ENERGY| Total", energy_child);
    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) {
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
    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) {
        std::regex pat("SCF run converged in");
        std::regex converge_pat("\\d+");
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, pat)) {
            std::regex_search(str, m2, converge_pat);
            info.get_child(m1.str(0)).push_back(pt::ptree::value_type("", m2.str(0)));
        }
    }});

}

} // namespace atomsciflow::cp2k::post
