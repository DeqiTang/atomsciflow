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

namespace atomsciflow::cp2k::post {

namespace fs = boost::filesystem;

Opt::Opt() {
    this->set_run("opt-out", "cp2k.out");
}

Opt::~Opt() {

}

void  Opt::read(const std::string& filepath) {

    auto get_start_time = [&](const std::string& str) {
        std::regex pat("STARTED\\ AT|ENDED\\ AT");
        std::regex time_pat("\\d{4}[-]\\d{2}[-]\\d{2}\\ [0-2][0-3]:[0-5][0-9]:[0-5][0-9]\\.[0-9]{3}");
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, pat)) {
            std::regex_search(str, m2, time_pat);
            this->info.put(m1.str(0), m2.str(0));
        }
    };

    pt::ptree energy_child;
    info.add_child("ENERGY| Total", energy_child);
    auto get_energy = [&](const std::string& str) {
        std::regex pat("ENERGY\\| Total");
        std::regex energy_pat("[-][0-9]+\\.\\d+");
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, pat)) {
            std::regex_search(str, m2, energy_pat);
            info.get_child(m1.str(0)).push_back(pt::ptree::value_type("", m2.str(0)));
        }
    };

    std::ifstream stream;
    stream.open(filepath);
    std::string line;

    while (std::getline(stream, line)) {
        get_start_time(line);
        get_energy(line);
    }

    stream.close();
}

void Opt::write(const std::string& directory) {
    pt::write_json((fs::path(directory) / "post-opt.json").string(), this->info);
}

void Opt::run(const std::string& directory) {
    this->read((fs::path(directory) / this->run_params["opt-out"]).string());
    fs::create_directory(fs::path(directory) / "post.dir");
    this->write((fs::path(directory) / "post.dir").string());
}

} // namespace atomsciflow::cp2k::post
