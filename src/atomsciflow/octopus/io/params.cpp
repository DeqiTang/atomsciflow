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

#include "atomsciflow/octopus/io/params.h"

#include <fstream>
#include <boost/algorithm/string.hpp>

namespace atomsciflow::octopus::io {

void read_params(Octopus& octopus, const std::string& filepath) {
    std::ifstream stream;
    stream.open(filepath);
    std::string line;
    std::vector<std::string> lines;

    while (std::getline(stream, line)) {
        lines.emplace_back(line);
    }

    std::vector<std::string> str_vec;
    std::string tmp_str;

    for (const auto& item : lines) {
        tmp_str = item;
        boost::erase_all(tmp_str, " ");
        boost::erase_all(tmp_str, "\t");
        if (tmp_str == "") {
            continue;
        }

        boost::split(str_vec, item, boost::is_any_of("="));
        tmp_str = str_vec[0];
        boost::erase_all(tmp_str, " ");
        boost::erase_all(tmp_str, "\t");
        
        if (boost::starts_with(tmp_str, "#")) {
            continue;
        }

        octopus.set_param(tmp_str, str_vec[1]);
    }

    stream.close();
}

} // namespace atomsciflow::octopus::io
