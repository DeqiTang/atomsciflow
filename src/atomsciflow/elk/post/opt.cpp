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

#include "atomsciflow/elk/post/opt.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

#include "atomsciflow/base/xyz.h"
#include "atomsciflow/utils/structure.h"

namespace atomsciflow::elk::post {

namespace fs = boost::filesystem;
namespace ba = boost::algorithm;

Opt::Opt() {
    this->set_run("program-out", "elk.out");
    this->set_run("output-json", "post-opt.json");

    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat("Elk code version\\s+\\d+\\.\\d+.\\d+\\s+started");
        std::regex version_pat("\\d+\\.\\d+\\.\\d+");
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, pat)) {
            std::regex_search(str, m2, version_pat);
            info.put("elk-version", m2.str(0));
        }
    }});

    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat("Total number of threads");
        std::regex threads_pat("\\d+");
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, pat)) {
            std::regex_search(str, m2, threads_pat);
            info.put("threads", m2.str(0));
        }
    }});
}

void Opt::get_extra_info(const std::string& directory) {
    std::ifstream stream;
    
    std::string line;

    std::vector<std::string> totenergy_opt_lines;
    stream.open((fs::path(directory) / "TOTENERGY_OPT.OUT").string());
    while (std::getline(stream, line)) {
        totenergy_opt_lines.push_back(line);
    }
    stream.close();

    std::vector<std::string> stress_opt_lines;
    stream.open((fs::path(directory) / "STRESS_OPT.OUT").string());
    while (std::getline(stream, line)) {
        stress_opt_lines.push_back(line);
    }
    stream.close();

    pt::ptree totenergy_child;
    info.add_child("tot-energy", totenergy_child);
    for (auto& item : totenergy_opt_lines) {
        info.get_child("tot-energy").push_back(pt::ptree::value_type("", boost::erase_all_copy(item, " ")));
    }

    std::regex stress_pat("\\-?\\d+\\.\\d+");
    std::smatch m1;
    pt::ptree stress_child;
    info.add_child("stress", stress_child);
    for (auto& item : stress_opt_lines) {
        if (std::regex_search(item, m1, stress_pat)) {
            info.get_child("stress").push_back(pt::ptree::value_type("", m1.str(0)));
        }
    }
}

void Opt::run(const std::string& directory) {
    this->get_extra_info(directory);
    Post::run(directory);

    std::string line;
    std::string tmp_str;
    std::vector<std::string> str_vec_1;
    std::vector<std::string> str_vec_2;

    std::ifstream stream;
    stream.open((fs::path(directory) / "GEOMETRY.OUT").string());
    std::vector<std::string> geometry_opt_lines;
    while (std::getline(stream, line)) {
        geometry_opt_lines.push_back(line);
    }
    stream.close();

    Xyz xyz;
    std::vector<double> avec;
    double scale;
    double scale1;
    double scale2;
    double scale3;
    for (int i = 0; i < geometry_opt_lines.size(); i++) {
        if (boost::contains(geometry_opt_lines[i], "avec")) {
            for (int j = 0; j < 3; j++) {
                boost::split(str_vec_1, geometry_opt_lines[i+j+1], boost::is_any_of(" "), boost::token_compress_on);
                avec.push_back(boost::lexical_cast<double>(str_vec_1[1]));
                avec.push_back(boost::lexical_cast<double>(str_vec_1[2]));
                avec.push_back(boost::lexical_cast<double>(str_vec_1[3]));
            }
        }
        if (boost::contains(geometry_opt_lines[i], "scale")) {
            scale = boost::lexical_cast<double>(boost::erase_all_copy(geometry_opt_lines[i+1], " "));
        }
        if (boost::contains(geometry_opt_lines[i], "scale1")) {
            scale1 = boost::lexical_cast<double>(boost::erase_all_copy(geometry_opt_lines[i+1], " "));
        }
        if (boost::contains(geometry_opt_lines[i], "scale2")) {
            scale2 = boost::lexical_cast<double>(boost::erase_all_copy(geometry_opt_lines[i+1], " "));
        }
        if (boost::contains(geometry_opt_lines[i], "scale3")) {
            scale3 = boost::lexical_cast<double>(boost::erase_all_copy(geometry_opt_lines[i+1], " "));
        }
    }

    double bohr_to_angstrom = 0.529177249;
    std::vector<double> factors;
    factors.push_back(bohr_to_angstrom * scale * scale1);
    factors.push_back(bohr_to_angstrom * scale * scale2);
    factors.push_back(bohr_to_angstrom * scale * scale3);
    xyz.cell.clear();
    for (int i = 0; i < 3; i++) {
        std::vector<double> vec;
        vec.push_back(avec[i*3] * factors[i]);
        vec.push_back(avec[i*3 + 1] * factors[i]);
        vec.push_back(avec[i*3 + 2] * factors[i]);
        xyz.cell.push_back(vec);
    }

    std::vector<Atom> atoms_frac;
    for (int i = 0; i < geometry_opt_lines.size(); i++) {
        if (boost::contains(geometry_opt_lines[i], ".in")) {
            boost::split(str_vec_1, geometry_opt_lines[i], boost::is_any_of("."), boost::token_compress_on);
            boost::split(str_vec_2, str_vec_1[0], boost::is_any_of("\'"), boost::token_compress_on);
            std::string name = str_vec_2[1];
            boost::split(str_vec_1, geometry_opt_lines[i+1], boost::is_any_of(" "), boost::token_compress_on);
            int natoms = boost::lexical_cast<int>(str_vec_1[1]);
            for (int j = 0; j < natoms; j++) {
                boost::split(str_vec_2, geometry_opt_lines[i+2+j], boost::is_any_of(" "), boost::token_compress_on);
                Atom atom;
                atom.name = name;
                atom.x = boost::lexical_cast<double>(str_vec_2[1]);
                atom.y = boost::lexical_cast<double>(str_vec_2[2]);
                atom.z = boost::lexical_cast<double>(str_vec_2[3]);
                atoms_frac.push_back(atom);
            }
        }
    }
    atoms_frac_to_cart(xyz.atoms, atoms_frac, xyz.cell);
    xyz.write_xyz_file((fs::path(directory) / "post.dir/optimized.xyz").string());
}

} // namespace atomsciflow::elk::post
