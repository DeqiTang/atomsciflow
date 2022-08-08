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

#include "atomsciflow/qe/post/opt.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#include "atomsciflow/base/xyz.h"
#include "atomsciflow/utils/structure.h"

namespace atomsciflow::qe::post {

namespace fs = boost::filesystem;
namespace ba = boost::algorithm;

Opt::Opt() {
    this->set_run("program-out", "pw.out");
    this->set_run("output-json", "post-opt.json");

    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat1("starts on");
        std::smatch m1;
        if (std::regex_search(str, m1, pat1)) {
            std::vector<std::string> str_vec;
            boost::split(str_vec, str, boost::is_any_of(" "), boost::token_compress_on);
            info.put("qe-version", str_vec[3]);
            info.put("start-date", str_vec[6]);
            info.put("start-time", str_vec[8]);
        }
    }});

    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat1("terminated on");
        std::smatch m1;
        if (std::regex_search(str, m1, pat1)) {
            std::vector<std::string> str_vec;
            boost::split(str_vec, str, boost::is_any_of(" "), boost::token_compress_on);
            info.put("end-date", str_vec[7]);
            info.put("end-time", str_vec[6]);
        }
    }});

    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat1("number of atoms/cell");
        std::regex pat2("\\d+");
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, pat1)) {
            std::regex_search(str, m2, pat2);
            info.put("num-atoms-per-cell", m2.str(0));
        }
    }});

    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat1("lattice parameter \\(alat\\)");
        std::regex pat2("\\d+\\.\\d+");
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, pat1)) {
            std::regex_search(str, m2, pat2);
            info.put("alat", m2.str(0));
        }
    }});

    pt::ptree energy_child;
    info.add_child("energy", energy_child);
    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) -> void {
        std::regex pat1("\\!\\ +total energy");
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

    std::ifstream stream;
    stream.open((fs::path(directory) / "pw.out").string());

    std::string line;
    std::vector<std::string> str_vec;
    std::vector<std::string> pw_out_lines;
    while (std::getline(stream, line)) {
        pw_out_lines.emplace_back(line);
    }
    stream.close();

    Xyz xyz;
    std::vector<Atom> atoms_frac;

    double bohr_to_angstrom = 0.529177249;
    xyz.cell.clear();
    for (int i = 0; i < pw_out_lines.size(); i++) {
        if (boost::contains(pw_out_lines[i], "crystal axes: (cart. coord. in units of alat)")) {
            for (int j = 0; j < 3; j++) {
                boost::split(str_vec, pw_out_lines[i+j+1], boost::is_any_of(" "), boost::token_compress_on);
                std::vector<double> vec;
                vec.push_back(boost::lexical_cast<double>(str_vec[4]) * info.get<double>("alat") * bohr_to_angstrom);
                vec.push_back(boost::lexical_cast<double>(str_vec[5]) * info.get<double>("alat") * bohr_to_angstrom);
                vec.push_back(boost::lexical_cast<double>(str_vec[6]) * info.get<double>("alat") * bohr_to_angstrom);
                xyz.cell.push_back(vec);
            }
        }
    }

    int index_final_pos_start = 0;
    int index_final_pos_end = 0;
    for (int i = pw_out_lines.size() - 1; i >=0; i--) {
        if (boost::contains(pw_out_lines[i], "End final coordinates")) {
            index_final_pos_end = i - 1;
            for (int j = i; j >= 0; j--) {
                if (boost::contains(pw_out_lines[j], "ATOMIC_POSITIONS (crystal)")) {
                    index_final_pos_start = j + 1;
                    break;
                }
            }
            break;
        }
    }
    for (int i = index_final_pos_start; i <= index_final_pos_end; i++) {
        boost::split(str_vec, pw_out_lines[i], boost::is_any_of(" "), boost::token_compress_on);
        Atom atom;
        atom.name = str_vec[0];
        atom.x = boost::lexical_cast<double>(str_vec[1]);
        atom.y = boost::lexical_cast<double>(str_vec[2]);
        atom.z = boost::lexical_cast<double>(str_vec[3]);
        atoms_frac.push_back(atom);
    }

    atoms_frac_to_cart(xyz.atoms, atoms_frac, xyz.cell);

    xyz.write_xyz_file((fs::path(directory) / "post.dir/optimized.xyz").string());
}

} // namespace atomsciflow::qe::post
