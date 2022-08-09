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

#include "atomsciflow/nwchem/post/opt.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

#include "atomsciflow/base/xyz.h"

namespace atomsciflow::nwchem::post {

namespace fs = boost::filesystem;
namespace ba = boost::algorithm;

Opt::Opt() {
    this->set_run("program-out", "nwchem.out");
    this->set_run("output-json", "post-opt.json");

    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat("Northwest Computational Chemistry Package");
        std::regex version_pat("\\d+\\.\\d+");
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, pat)) {
            std::regex_search(str, m2, version_pat);
            info.put("nwchem-version", m2.str(0));
        }
    }});

    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat1("date\\s+\\=");
        std::regex pat2("[A-Za-z]+\\s+[A-Za-z]+\\s+\\d+\\s+\\d+\\:\\d+\\:\\d+\\s+\\d{4}");
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, pat1)) {
            std::regex_search(str, m2, pat2);
            info.put("start-time", m2.str(0));
        }
    }});

    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat1("Total times  cpu");
        std::regex pat2("\\d+\\.\\d+s");
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, pat1)) {
            std::regex_search(str, m2, pat2);
            info.put("total-time", m2.str(0));
        }
    }});

    pt::ptree energy_child;
    info.add_child("energy", energy_child);
    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat1("Total DFT energy =");
        std::regex pat2("\\-?\\d+\\.\\d+");
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, pat1)) {
            std::regex_search(str, m2, pat2);
            info.get_child("energy").push_back(pt::ptree::value_type("", m2.str(0)));
        }
    }});

}

void Opt::run(const std::string& directory) {
    Post::run(directory);
    //TODO:
    // extract the optimized structure    
}

} // namespace atomsciflow::nwchem::post
