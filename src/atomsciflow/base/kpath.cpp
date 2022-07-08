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

/// @file src/atomsciflow/base/kpath.cpp
/// @author Deqi Tang
/// Mail: deqitang@gmail.com
/// Created Time: Fri 04 Mar 2022 10:33:26 PM CST

#include "atomsciflow/base/kpath.h"

#include <boost/algorithm/string.hpp>
#include <string>
#include <fstream>

namespace atomsciflow {

namespace ba = boost::algorithm;

/**
 * @param kpath a vector of string
 * input of kpath in format like
 *  {
 *      "0.000000 0.000000 0.000000 GAMMA 5",
 *      "0.500000 0.000000 0.000000 X 5",
 *      "0.000000 0.000000 0.500000 A 10",
 *      "0.500000 0.500000 0.500000 R |"
 *  }
 */
void Kpath::read(const std::vector<std::string>& kpath) {
    std::vector<std::string> vec_str;

    for (const auto& kpoint : kpath) {
        boost::split(vec_str, kpoint, boost::is_space());
        if (vec_str[4] != "|") {
            this->add_point(
                std::atof(vec_str[0].c_str()),
                std::atof(vec_str[1].c_str()),
                std::atof(vec_str[2].c_str()),
                ba::to_upper_copy(vec_str[3]),
                int(std::atof(vec_str[4].c_str())) 
            );
        } else {
            this->add_point(
                std::atof(vec_str[0].c_str()),
                std::atof(vec_str[1].c_str()),
                std::atof(vec_str[2].c_str()),
                ba::to_upper_copy(vec_str[3]),
                0
            );
        }
    }
}

/**
 * @param kpath a string containging the kpath information
 * in format like
 *  "0.000000 0.000000 0.000000 GAMMA 5;0.500000 0.000000 0.000000 X 5;0.000000 0.000000 0.500000 A |;0.500000 0.500000 0.500000 R |"
 */
void Kpath::read(const std::string& kpath) {
    std::vector<std::string> vec_str;
    boost::split(vec_str, kpath, boost::is_any_of(";"));
    this->read(vec_str);
}

/**
 * @param kpath_file the kpath file path
 *  input kpath by reading from the file
 *  which is in format like this
 *  5
 *  0.0 0.0 0.0 #GAMMA 15
 *  x.x x.x x.x #XXX |
 *  x.x x.x x.x #XXX 10
 *  x.x x.x x.x #XXX 15
 *  x.x x.x x.x #XXX |
 */
void Kpath::read_file(const std::string& kpath_file) {
    std::vector<std::string> vec_str;
    std::vector<std::string> vec_str_1;
    std::vector<std::string> vec_str_2;
    std::vector<std::string> vec_str_3;

    std::ifstream kpath_file_stream;
    std::string line;
    std::vector<std::string> lines;
    kpath_file_stream.open(kpath_file);
    while (std::getline(kpath_file_stream, line)) {
        lines.emplace_back(line);
    }
    kpath_file_stream.close();
    boost::split(vec_str, lines[0], boost::is_any_of("\n"));
    int nk = int(std::atof(vec_str[0].c_str()));
    for (int i = 0; i < nk; i++) {
        boost::split(vec_str_1, lines[i+1], boost::is_any_of("\n"));
        boost::split(vec_str_2, vec_str_1[0], boost::is_any_of(" "));
        if (vec_str_2[4] != "|") {
            boost::split(vec_str, lines[i+1], boost::is_space());
            boost::split(vec_str_3, vec_str[3], boost::is_any_of("#"));
            this->add_point(
                std::atof(vec_str[0].c_str()),
                std::atof(vec_str[1].c_str()),
                std::atof(vec_str[2].c_str()),
                ba::to_upper_copy(vec_str_3[1]),
                int(std::atof(vec_str[4].c_str()))
            );
        } else if (vec_str_2[4] == "|") {
            boost::split(vec_str, lines[i+1], boost::is_space());
            boost::split(vec_str_3, vec_str[3], boost::is_any_of("#"));
            this->add_point(
                std::atof(vec_str[0].c_str()),
                std::atof(vec_str[1].c_str()),
                std::atof(vec_str[2].c_str()),
                ba::to_upper_copy(vec_str_3[1]),
                0
            );
        }
    }
}

Kpath get_kpath(const std::vector<std::string>& kpath) {
    atomsciflow::Kpath out{};
    out.read(kpath);
    return out;
}

Kpath get_kpath(const std::string& kpath) {
    atomsciflow::Kpath out{};
    out.read(kpath);
    return out;
}

Kpath get_kpath(const std::vector<std::string>& kpath_manual, const std::string& kpath_file) {

    atomsciflow::Kpath kpath{};

    if (kpath_manual.size() != 0 ) {
        kpath =get_kpath(kpath_manual);
    } else if (kpath_file != "") {
        kpath = get_kpath(kpath_file);
    }

    return kpath;
}

} // namespace atomsciflow
