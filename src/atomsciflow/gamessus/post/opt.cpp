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

#include "atomsciflow/gamessus/post/opt.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

#include "atomsciflow/base/xyz.h"

namespace atomsciflow::gamessus::post {

namespace fs = boost::filesystem;
namespace ba = boost::algorithm;

Opt::Opt() {
    this->set_run("program-out", "gamessus.out");
    this->set_run("output-json", "post-opt.json");

    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat("GAMESS VERSION =");
        std::smatch m1;
        if (std::regex_search(str, m1, pat)) {
            std::vector<std::string> str_vec_1;
            std::vector<std::string> str_vec_2;
            boost::split(str_vec_1, str, boost::is_any_of("="), boost::token_compress_on);
            boost::split(str_vec_2, str_vec_1[1], boost::is_any_of("*"), boost::token_compress_on);
            boost::split(str_vec_1, str_vec_2[0], boost::is_any_of(" "), boost::token_compress_on);
            std::string tmp_str = boost::join(str_vec_1, " ");
            boost::erase_first(tmp_str, " ");
            boost::erase_last(tmp_str, " ");
            info.put("gamessus-version", tmp_str);
        }
    }});

    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat1("EXECUTION OF GAMESS BEGUN");
        std::regex pat2("\\d+\\:\\d+\\:\\d+");
        std::regex pat3("\\d+\\-[A-Z-a-z]+\\-\\d{4}");
        std::smatch m1;
        std::smatch m2;
        std::smatch m3;
        if (std::regex_search(str, m1, pat1)) {
            std::regex_search(str, m2, pat2);
            std::regex_search(str, m3, pat3);
            info.put("start-time", m2.str(0));
            info.put("start-date", m3.str(0));
        }
    }});

    pt::ptree energy_child;
    info.add_child("energy", energy_child);
    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat1("TOTAL ENERGY =");
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

} // namespace atomsciflow::gamessus::post
