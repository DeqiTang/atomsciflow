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

#include "atomsciflow/octopus/post/opt.h"

#include <boost/algorithm/string.hpp>
// #include <boost/algorithm/string/replace.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>

#include "atomsciflow/base/xyz.h"

namespace atomsciflow::octopus::post {

namespace fs = boost::filesystem;
namespace ba = boost::algorithm;

Opt::Opt() {
   this->set_run("program-out", "octopus.out");
    this->set_run("output-json", "post-opt.json");

    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat("Version\\s+\\:\\s+\\d+\\.\\d+");
        std::regex version_pat("\\d+\\.\\d+");
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, pat)) {
            std::regex_search(str, m2, version_pat);
            info.put("octopus-version", m2.str(0));
        }
    }});

    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat("Calculation started on");
        std::regex date_pat("\\d{4}/\\d{2}/\\d{2}");
        std::regex time_pat("\\d{2}:\\d{2}:\\d{2}");
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, pat)) {
            std::regex_search(str, m2, date_pat);
            info.put("start-date", m2.str(0));
            std::regex_search(str, m2, time_pat);
            info.put("start-time", m2.str(0));
        }
    }});

    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat("Calculation ended on");
        std::regex date_pat("\\d{4}/\\d{2}/\\d{2}");
        std::regex time_pat("\\d{2}:\\d{2}:\\d{2}");
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, pat)) {
            std::regex_search(str, m2, date_pat);
            info.put("end-date", m2.str(0));
            std::regex_search(str, m2, time_pat);
            info.put("end-time", m2.str(0));
        }
    }});

    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat1("Octopus will run in \\d+ dimension");
        std::regex pat2("\\d+");
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, pat1)) {
            std::regex_search(str, m2, pat2);
            info.put("dimension", m2.str(0));
        }
    }});

    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat1("Symmetry group");
        std::smatch m1;
        if (std::regex_search(str, m1, pat1)) {
            std::vector<std::string> vec_str;
            boost::split(vec_str, str, boost::is_any_of(":"), boost::token_compress_on);
            std::string group = vec_str[1];
            boost::erase_all(group, " ");
            info.put("symmetry-group", group);
        }
    }});

    pt::ptree energy_child;
    info.add_child("energy", energy_child);
    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat1("Energy\\s+\\=\\s+\\-?\\d+\\.\\d+\\s+");
        std::smatch m1;
        if (std::regex_search(str, m1, pat1)) {
            std::vector<std::string> vec_str;
            boost::split(vec_str, str, boost::is_any_of(" "), boost::token_compress_on);
            info.get_child("energy").push_back(pt::ptree::value_type("", vec_str[3]));
        }
    }});

    pt::ptree max_force_child;
    info.add_child("max-force", max_force_child);
    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat1("Max force\\s+\\=\\s+\\-?\\d+\\.\\d+\\s+");
        std::smatch m1;
        if (std::regex_search(str, m1, pat1)) {
            std::vector<std::string> vec_str;
            boost::split(vec_str, str, boost::is_any_of(" "), boost::token_compress_on);
            info.get_child("max-force").push_back(pt::ptree::value_type("", vec_str[4]));
        }
    }});
    
    pt::ptree scf_convergence_child;
    info.add_child("scf-convergence", scf_convergence_child);
    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat1("Info\\:\\s+SCF converged in\\s+\\d+\\s+iterations");
        std::regex pat2("\\d+");
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, pat1)) {
            std::regex_search(str, m2, pat2);
            info.get_child("scf-convergence").push_back(pt::ptree::value_type("", m2.str(0)));
        }
    }});

    pt::ptree max_dr_child;
    info.add_child("max-dr", max_dr_child);
    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat1("Max dr\\s+\\=\\s+\\-?\\d+\\.\\d+\\s+");
        std::smatch m1;
        if (std::regex_search(str, m1, pat1)) {
            std::vector<std::string> vec_str;
            boost::split(vec_str, str, boost::is_any_of(" "), boost::token_compress_on);
            info.get_child("max-dr").push_back(pt::ptree::value_type("", vec_str[4]));
        }
    }});

}

void Opt::run(const std::string& directory) {
    Post::run(directory);
    
    std::vector<std::string> str_vec_1;
    
    std::ifstream stream;
    stream.open((fs::path(directory) / "inp").string());

    std::string line;
    std::string tmp_str;
    std::vector<std::string> inp_lines;
    while (std::getline(stream, line)) {
        inp_lines.push_back(line);
    }
    stream.close();

    Xyz final_xyz;
    final_xyz.cell.clear();
    for (int i = 0; i < inp_lines.size(); i++) {
        if (boost::contains(inp_lines[i], "LatticeVectors")) {
            for (int j = 0; j < 3; j++) {
                boost::split(str_vec_1, inp_lines[i+j+1], boost::is_any_of("|"), boost::token_compress_on);
                std::vector<double> vec;
                for (auto& item : str_vec_1) {
                    boost::erase_all(item, " ");
                }                
                vec.push_back(boost::lexical_cast<double>(str_vec_1[0]));
                vec.push_back(boost::lexical_cast<double>(str_vec_1[1]));
                vec.push_back(boost::lexical_cast<double>(str_vec_1[2]));
                final_xyz.cell.push_back(vec);
            }
            break;
        }
    }

    stream.open((fs::path(directory) / "min.xyz").string());

    std::vector<std::string> min_xyz_lines;
    while (std::getline(stream, line)) {
        min_xyz_lines.push_back(line);
    }
    stream.close();

    int natom = boost::lexical_cast<int>(boost::erase_all_copy(min_xyz_lines[0], " "));
    final_xyz.atoms.clear();
    for (int i = 0; i < natom; i++) {
        Atom atom;
        boost::split(str_vec_1, min_xyz_lines[i+2], boost::is_any_of(" "), boost::token_compress_on);
        for (auto& item : str_vec_1) {
            boost::erase_all(item, " ");
            boost::erase_all(item, "\t");
        }
        if (str_vec_1[0] != "") {
            atom.name = boost::lexical_cast<std::string>(str_vec_1[0]);
            atom.x = boost::lexical_cast<double>(str_vec_1[1]);
            atom.y = boost::lexical_cast<double>(str_vec_1[2]);
            atom.z = boost::lexical_cast<double>(str_vec_1[3]);
        } else { // for the line of atom in the xyz file begin with white space or tab
            atom.name = boost::lexical_cast<std::string>(str_vec_1[1]);
            atom.x = boost::lexical_cast<double>(str_vec_1[2]);
            atom.y = boost::lexical_cast<double>(str_vec_1[3]);
            atom.z = boost::lexical_cast<double>(str_vec_1[4]);
        }
        final_xyz.atoms.push_back(atom);
    }

    final_xyz.write_xyz_file((fs::path(directory) / "post.dir/optimized.xyz").string());
}

} // namespace atomsciflow::octopus::post
