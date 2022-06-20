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

/// @file src/atomsciflow/cp2k/cp2k.cpp
/// @author deqi
/// Mail: deqi_tang@163.com
/// Created Time: Sun 31 Jan 2021 07:28:30 PM CST

#include "atomsciflow/cp2k/cp2k.h"

#include <iostream>
#include <fstream>
//#include <filesystem>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "atomsciflow/server/submit_script.h"
#include "atomsciflow/remote/server.h"

namespace atomsciflow::cp2k {

namespace fs = boost::filesystem;
namespace ba = boost::algorithm;

Cp2k::Cp2k() {

    this->new_section("global");
    this->sections["global"]->set_param("project", "cp2k_job");
    this->sections["global"]->set_param("print_level", "low");
    this->sections["global"]->set_param("run_type", "energy_force");

    this->new_section("force_eval");
    this->sections["force_eval"]->set_param("method", "quickstep");

    //Cp2kSection dft{"dft"};
    auto dft = std::make_shared<Cp2kSection>("dft");
    dft->set_param("basis_set_file_name", "BASIS_MOLOPT");
    dft->set_param("potential_file_name", "GTH_POTENTIALS");

    auto& qs = dft->add_section("qs");
    qs->set_param("eps_default", "1.0e-14");

    auto& mgrid = dft->add_section("mgrid");
    mgrid->set_param("ngrids", 4);
    mgrid->set_param("cutoff", 100);
    mgrid->set_param("rel_cutoff", 60);

    auto& xc = dft->add_section("xc");
    auto& xc_functional = xc->add_section("xc_functional");
    xc_functional->section_parameter = "pbe";

    auto& scf = dft->add_section("scf");
    scf->set_param("scf_guess", "atomic");
    scf->set_param("eps_scf", "1.0e-7");
    scf->set_param("max_scf", 100);
    auto& diag = scf->add_section("diagonalization");
    diag->set_param("algorithm", "standard");

    auto& mixing = scf->add_section("mixing");
    mixing->set_param("method", "broyden_mixing");
    mixing->set_param("alpha", 0.4);
    mixing->set_param("nbroyden", 8);

    auto& print = dft->add_section("print");

    this->sections["force_eval"]->add_section("dft", dft);

    job.set_run("cmd", "$ASF_CMD_CP2K");
    job.set_run("input", "cp2k.inp");
    job.set_run("output", "cp2k.out");

    job.set_run("script_name_head", "cp2k-run");

    this->plumed = std::make_shared<plumed::Plumed>();
}

Cp2k::~Cp2k() {

}

/**
 * @brief 
 * 
 * @param path The path to the section, like "force_eval/dft/scf"
 */
void Cp2k::new_section(const std::string& path) {
    // if (this->exists_section(path)) {
    //     return;
    // }

    std::vector<std::string> vec_str;
    ba::split(vec_str, path, boost::is_any_of("/"));
    int length =vec_str.size();

    auto new_recursive = [&] (auto&& _new_recursive, std::map<std::string, std::shared_ptr<Cp2kSection>>& _sections, int i) {
        std::string sec_name = "";
        std::vector<std::string> vec_str_1;
        std::vector<std::string> vec_str_2;
        if (boost::contains(vec_str[i], "(")) {
            boost::split(vec_str_1, vec_str[i], boost::is_any_of("("));
            sec_name = vec_str_1[0];
        } else {
            sec_name = vec_str[i];
        }

        if (_sections.find(sec_name) == _sections.end()) {
            _sections[sec_name] = std::make_shared<Cp2kSection>(sec_name);
            if (boost::contains(vec_str[i], "(")) {
                boost::split(vec_str_1, vec_str[i], boost::is_any_of("("));
                boost::split(vec_str_2, vec_str_1[1], boost::is_any_of(")"));
                _sections[sec_name]->section_parameter = vec_str_2[0];
            }
        }

        if (i + 1 >= length) {
            return;
        }
        _new_recursive(_new_recursive, _sections[sec_name]->sections, i + 1);
    };
    new_recursive(new_recursive, this->sections, 0);

    // auto& sec = this->sections;
    // int i = 0;
    // std::string sec_name = "";
    // std::vector<std::string> vec_str_1;
    // std::vector<std::string> vec_str_2;
    // while (i < length) {
    //     if (boost::contains(vec_str[i], "(")) {
    //         boost::split(vec_str_1, vec_str[i], boost::is_any_of("("));
    //         sec_name = vec_str_1[0];
    //     } else {
    //         sec_name = vec_str[i];
    //     }
    //     if (sec.find(sec_name) == sec.end()) {
    //         sec[sec_name] = std::make_shared<Cp2kSection>(sec_name);
    //         if (boost::contains(vec_str[i], "(")) {
    //             boost::split(vec_str_1, vec_str[i], boost::is_any_of("("));
    //             boost::split(vec_str_2, vec_str_1[1], boost::is_any_of(")"));
    //             sec[sec_name]->section_parameter = vec_str_2[0];
    //         }
    //     }
    //     sec = sec[sec_name]->sections;
    //     i++;
    // }

    return;
}

bool Cp2k::exists_section(const std::string& path) {

    bool exists = true;

    std::vector<std::string> vec_str;
    ba::split(vec_str, path, boost::is_any_of("/"));
    int length = vec_str.size();

    int i = 0;
    auto sec = this->sections;
    while (i < length) {
        std::string section_name = "";

        if (boost::contains(vec_str[i], "(")) {
            std::vector<std::string> vec_str_1;
            boost::split(vec_str_1, vec_str[i], boost::is_any_of("("));
            section_name = vec_str_1[0];
        } else {
            section_name = vec_str[i];
        }

        if (0 == sec.count(section_name)) {
            exists = false;
            break;
        }
        sec = sec[section_name]->sections;
        i++;
    }

    return exists;
}

template <typename T>
void Cp2k::set_param(const std::string& path, T value) {
    std::vector<std::string> vec_str;
    ba::split(vec_str, path, boost::is_any_of("/"));

    std::vector<std::string> vec_sections = vec_str;
    vec_sections.pop_back();
    if (false == this->exists_section(ba::join(vec_sections, "/"))) {
        this->new_section(ba::join(vec_sections, "/"));
    }

    int i = 0;
    auto sec = this->sections;
    int length = vec_str.size();
    while (i < length) {
        std::string sec_name = vec_str[i];
        std::vector<std::string> vec_str_1;
        std::vector<std::string> vec_str_2;
        if (boost::contains(vec_str[i], "(")) {
            boost::split(vec_str_1, vec_str[i], boost::is_any_of("("));
            boost::split(vec_str_2, vec_str_1[1], boost::is_any_of(")"));
            sec_name = vec_str_1[0];
            sec[sec_name]->section_parameter = vec_str_2[0];
        }
        if (i == (length - 2)) {
            sec[sec_name]->set_param(vec_str[i+1], value);
            break;
        }
        sec = sec[sec_name]->sections;
        i++;
    }

    return;
}

void Cp2k::py_set_param(const std::string& path, int value) {
    this->set_param(path, value);
}

void Cp2k::py_set_param(const std::string& path, double value) {
    this->set_param(path, value);
}

void Cp2k::py_set_param(const std::string& path, std::string value) {
    this->set_param(path, value);
}

void Cp2k::py_set_param(const std::string& path, std::vector<int> value) {
    this->set_param(path, value);
}

void Cp2k::py_set_param(const std::string& path, std::vector<double> value) {
    this->set_param(path, value);
}

void Cp2k::py_set_param(const std::string& path, std::vector<std::string> value) {
    this->set_param(path, value);
}

void Cp2k::py_set_param(const std::string& path, std::vector<std::vector<int>> value) {
    this->set_param(path, value);
}

void Cp2k::py_set_param(const std::string& path, std::vector<std::vector<double>> value) {
    this->set_param(path, value);
}

void Cp2k::py_set_param(const std::string& path, std::vector<std::vector<std::string>> value) {
    this->set_param(path, value);
}

std::string Cp2k::to_string() {
    std::string out = "";
    for (const auto& item : this->sections) {
        out += this->sections[item.first]->to_string("  ") + "\n";
        out += "\n";
    }
    return out;
}

void Cp2k::get_xyz(const std::string &xyzfile) {
    this->xyz.read_xyz_file(xyzfile);
    this->set_subsys();
    job.set_run("xyz_file", fs::absolute(xyzfile).string());
}

std::shared_ptr<Cp2kSection>& Cp2k::set_subsys(Xyz& xyz) {
    this->xyz = xyz;
    return this->set_subsys();
}

std::shared_ptr<Cp2kSection>& Cp2k::set_subsys() {
    auto& subsys = this->sections["force_eval"]->add_section("subsys"); 
    auto& cell = subsys->add_section("cell");
    cell->set_param("a", this->xyz.cell[0]);
    cell->set_param("b", this->xyz.cell[1]);
    cell->set_param("c", this->xyz.cell[2]);
    
    auto& coord = subsys->add_section("coord");
    std::vector<std::vector<std::string>> matrix_str;
    for (const auto& atom : this->xyz.atoms) {
        matrix_str.push_back(std::vector<std::string>{
            atom.name,
            std::to_string(atom.x),
            std::to_string(atom.y),
            std::to_string(atom.z),
        });
    }
    coord->section_var.set("", matrix_str);
    for (const auto& element : this->xyz.elements_set) {
        auto kind_name = (boost::format("kind[%1%]") % element).str();
        this->sections["force_eval"]->sections["subsys"]->add_section(
        kind_name);
        //this->sections["force_eval"].sections["subsys"].sections[kind_name].set_param("basis_set", "DZVP-MOLOPT-SR-GTH");
        //this->sections["force_eval"].sections["subsys"].sections[kind_name].set_param("potential", "GTH-PBE");
        this->set_param((boost::format("force_eval/subsys/%1%/basis_set") % kind_name).str(), "DZVP-MOLOPT-SR-GTH");
        this->set_param((boost::format("force_eval/subsys/%1%/potential") % kind_name).str(), "GTH-PBE");
        this->sections["force_eval"]->sections["subsys"]->sections[kind_name]->section_parameter = element;
    }
    return subsys;
}

void Cp2k::set_kpoint_set(const Kpath& kpath) {
    this->new_section("force_eval/dft/print/band_structure");

    int kpoint_set_i = 0;
    for (int i = 0; i < kpath.labels.size() - 1; i++) {
        if (kpath.links[i] != 0) {
            this->set_param(
                (
                    boost::format(
                        "force_eval/dft/print/band_structure/kpoint_set[%1%]/special_point[%2%]"
                    ) % kpoint_set_i % 0
                ).str(), 
                std::vector<std::string>{
                boost::lexical_cast<std::string>(kpath.labels[i]),
                boost::lexical_cast<std::string>(kpath.coords[i][0]),
                boost::lexical_cast<std::string>(kpath.coords[i][1]),
                boost::lexical_cast<std::string>(kpath.coords[i][2]),
            });     
            this->set_param(
                (
                    boost::format(
                        "force_eval/dft/print/band_structure/kpoint_set[%1%]/special_point[%2%]"
                    ) % kpoint_set_i % 1
                ).str(), 
                std::vector<std::string>{
                boost::lexical_cast<std::string>(kpath.labels[i+1]),
                boost::lexical_cast<std::string>(kpath.coords[i+1][0]),
                boost::lexical_cast<std::string>(kpath.coords[i+1][1]),
                boost::lexical_cast<std::string>(kpath.coords[i+1][2]),
            });
            this->set_param(
                (
                    boost::format(
                        "force_eval/dft/print/band_structure/kpoint_set[%1%]/npoints"
                    ) % kpoint_set_i
                ).str(),
                kpath.links[i] - 1
            );        
            this->set_param(
                (
                    boost::format(
                        "force_eval/dft/print/band_structure/kpoint_set[%1%]/units"
                    ) % kpoint_set_i
                ).str(),
                "B_VECTOR"
            );
            kpoint_set_i += 1;                            
        }
    } 
}

/**
 * @brief Cp2k::run The running entry for the calculation
 * @param directory The path to the directory where the calculation
 *  is running.
 * The running of the calculation is actually controlled
 * by the JobScheduler.
 */
void Cp2k::run(const std::string& directory) {
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << boost::format("cat >%1%<<EOF\n") % job.run_params["input"];
    step << this->to_string();
    step << "EOF\n";
    step << boost::format("$CMD_HEAD %1% -in %2% | tee %3%  \n") 
        % job.run_params["cmd"] 
        % job.run_params["input"] 
        % job.run_params["output"]
        ;
    job.steps.push_back(step.str());
    step.clear();

    job.run(directory);
}

// template explicit instantiation
template void Cp2k::set_param<int>(const std::string&, int);
template void Cp2k::set_param<double>(const std::string&, double);
template void Cp2k::set_param<std::string>(const std::string&, std::string);
template void Cp2k::set_param<std::vector<int>>(const std::string&, std::vector<int>);
template void Cp2k::set_param<std::vector<double>>(const std::string&, std::vector<double>);
template void Cp2k::set_param<std::vector<std::string>>(const std::string&, std::vector<std::string>);
template void Cp2k::set_param<std::vector<std::vector<int>>>(const std::string&, std::vector<std::vector<int>>);
template void Cp2k::set_param<std::vector<std::vector<double>>>(const std::string&, std::vector<std::vector<double>>);
template void Cp2k::set_param<std::vector<std::vector<std::string>>>(const std::string&, std::vector<std::vector<std::string>>);

} // namespace atomsciflow::cp2k
