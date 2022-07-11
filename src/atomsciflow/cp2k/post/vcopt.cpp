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

#include "atomsciflow/cp2k/post/vcopt.h"

#include <regex>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include "atomsciflow/cp2k/post/utils.h"

namespace atomsciflow::cp2k::post {

namespace fs = boost::filesystem;
namespace ba = boost::algorithm;

VcOpt::VcOpt() {
    this->set_run("cp2k-out", "cp2k.out");
    this->set_run("output-json", "post-vcopt.json");

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

    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) {
        std::regex pat1{"Mixing method\\:"};
        std::smatch m1;
        std::vector<std::string> vec_str;
        if (std::regex_search(str, m1, pat1)) {
            boost::split(vec_str, str, boost::is_any_of(" "), boost::token_compress_on);
            info.put(m1.str(0), vec_str[3]);
        }
    }});

    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) {
        std::regex pat1("eps_scf\\:");
        std::smatch m1;
        std::vector<std::string> vec_str;
        if (std::regex_search(str, m1, pat1)) {
            boost::split(vec_str, str, boost::is_any_of(" "), boost::token_compress_on);
            info.put(m1.str(0), vec_str[2]);
        }
    }});

    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) {
        std::regex pat1("eps_diis\\:");
        std::smatch m1;
        std::vector<std::string> vec_str;
        if (std::regex_search(str, m1, pat1)) {
            boost::split(vec_str, str, boost::is_any_of(" "), boost::token_compress_on);
            info.put(m1.str(0), vec_str[2]);
        }
    }});

    pt::ptree energy_child;
    info.add_child("ENERGY| Total", energy_child);
    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) {
        std::regex pat1("ENERGY\\| Total");
        std::regex pat2("[-][0-9]+\\.\\d+");
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, pat1)) {
            std::regex_search(str, m2, pat2);
            info.get_child(m1.str(0)).push_back(pt::ptree::value_type("", m2.str(0)));
        }
    }});

    pt::ptree converge_child;
    info.add_child("SCF run converged in", converge_child);
    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) {
        std::regex pat1("SCF run converged in");
        std::regex pat2("\\d+");
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, pat1)) {
            std::regex_search(str, m2, pat2);
            info.get_child(m1.str(0)).push_back(pt::ptree::value_type("", m2.str(0)));
        }
    }});

}

void VcOpt::run(const std::string& directory) {
    Post::run(directory);
    
    std::ifstream stream;
    stream.open((fs::path(directory) / "cell-optimization-pos-1.xyz").string());

    std::vector<std::string> vec_str;

    std::vector<std::string> traj_lines;
    std::string line;
    while (std::getline(stream, line)) {
        traj_lines.push_back(line);
    }
    stream.close();

    std::string tmp_str = traj_lines[0];
    ba::replace_all(tmp_str, "\n", "");
    ba::replace_all(tmp_str, " ", "");
    int natoms = boost::lexical_cast<int>(tmp_str);

    stream.open((fs::path(directory) / "cell-optimization-1.cell").string());
    std::vector<std::string> cell_lines;
    while (std::getline(stream, line)) {
        tmp_str = line;
        ba::replace_all(tmp_str, "\n", "");
        ba::replace_all(tmp_str, " ", "");
        ba::replace_all(tmp_str, "\t", "");
        if (tmp_str != "") {
            cell_lines.push_back(line);
        }
    }
    stream.close();

    std::ofstream out;
    out.open((fs::path(directory) / run_params["post-dir"] / "optimized.xyz").string());
    out << boost::format("%1%\n") % natoms;
    tmp_str = cell_lines[cell_lines.size() - 1];
    ba::replace_all(tmp_str, "\t", " ");
    ba::replace_all(tmp_str, "\n", " ");
    boost::split(vec_str, tmp_str, boost::is_any_of(" "), boost::token_compress_on);
    out << boost::format("cell: %1% %2% %3% | %4% %5% %6% | %7% %8% %9%\n")
        % vec_str[3] % vec_str[4] % vec_str[5]
        % vec_str[6] % vec_str[7] % vec_str[8]
        % vec_str[9] % vec_str[10] % vec_str[11];
    int traj_lines_size = traj_lines.size();
    for (int i = (natoms+2) * (int(traj_lines_size / (natoms+2))-1) + 2; i < (natoms+2) * (int(traj_lines_size / (natoms+2))); i++) {
        out << traj_lines[i] << "\n";
    }
    out.close();

}

} // namespace atomsciflow::cp2k::post
