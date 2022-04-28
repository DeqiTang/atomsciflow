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

namespace atomsciflow {

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
        if (_sections.find(vec_str[i]) == _sections.end()) {
            _sections[vec_str[i]] = std::make_shared<Cp2kSection>(vec_str[i]);
        }
        if (i + 1 >= length) {
            return;
        }
        _new_recursive(_new_recursive, _sections[vec_str[i]]->sections, i + 1);
    };
    new_recursive(new_recursive, this->sections, 0);

    // auto& sec = this->sections;
    // int i = 0;
    // std::string mid_name = "";
    // while (i < length) {
    //     mid_name = vec_str[i];
    //     if (sec.find(mid_name) == sec.end()) {
    //         sec[mid_name] = std::make_shared<Cp2kSection>(mid_name);
    //         std::cout << "Cp2k::new_section -> sec[?].name: " << sec[mid_name]->name << "\n";
    //     }
    //     std::cout << "Cp2k::new_section -> sec.size(): " << sec.size() << "\n";
    //     sec = sec[mid_name]->sections;
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
        if (0 == sec.count(vec_str[i])) {
            exists = false;
            break;
        }
        sec = sec[vec_str[i]]->sections;
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

    switch (vec_str.size()) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            this->sections[vec_str[0]]->set_param(vec_str[1], value);
            break;
        case 3:
            this->sections[vec_str[0]]->sections[vec_str[1]]->set_param(vec_str[2], value);
            break;
        case 4:
            this->sections[vec_str[0]]->sections[vec_str[1]]->sections[vec_str[2]]->set_param(vec_str[3], value);
            break;
        case 5:
            this->sections[vec_str[0]]->sections[vec_str[1]]->sections[vec_str[2]]->sections[vec_str[3]]->set_param(vec_str[4], value);
            break;
        case 6:
            this->sections[vec_str[0]]->sections[vec_str[1]]->sections[vec_str[2]]->sections[vec_str[3]]->sections[vec_str[4]]->set_param(vec_str[5], value);
            break; 
        case 7:
            this->sections[vec_str[0]]->sections[vec_str[1]]->sections[vec_str[2]]->sections[vec_str[3]]->sections[vec_str[4]]->sections[vec_str[5]]->set_param(vec_str[6], value);
            break;  
        case 8:
            this->sections[vec_str[0]]->sections[vec_str[1]]->sections[vec_str[2]]->sections[vec_str[3]]->sections[vec_str[4]]->sections[vec_str[5]]->sections[vec_str[6]]->set_param(vec_str[7], value);
            break;  
        case 9:
            this->sections[vec_str[0]]->sections[vec_str[1]]->sections[vec_str[2]]->sections[vec_str[3]]->sections[vec_str[4]]->sections[vec_str[5]]->sections[vec_str[6]]->sections[vec_str[7]]->set_param(vec_str[8], value);
            break;                                           
        default:
            break;
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
    this->set_job_steps_default();
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
    int i = 0;
    for (const auto& element : this->xyz.elements_set) {
        auto kind_name = (boost::format("kind[%1%]") % i).str();
        this->sections["force_eval"]->sections["subsys"]->add_section(
        kind_name);
        //this->sections["force_eval"].sections["subsys"].sections[kind_name].set_param("basis_set", "DZVP-MOLOPT-SR-GTH");
        //this->sections["force_eval"].sections["subsys"].sections[kind_name].set_param("potential", "GTH-PBE");
        this->set_param((boost::format("force_eval/subsys/%1%/basis_set") % kind_name).str(), "DZVP-MOLOPT-SR-GTH");
        this->set_param((boost::format("force_eval/subsys/%1%/potential") % kind_name).str(), "GTH-PBE");
        this->sections["force_eval"]->sections["subsys"]->sections[kind_name]->section_parameter = element;
        i++;
    }
    return subsys;
}

void Cp2k::set_job_steps_default() {
    job.steps.clear();
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << boost::format("cat >%1%<<EOF\n") % job.run_params["input"];
    step << this->to_string();
    step << "EOF\n";
    step << boost::format("$CMD_HEAD %1% -in %2% | tee %3%  \n") % job.run_params["cmd"] % job.run_params["input"] % job.run_params["output"];
    job.steps.push_back(step.str());
    step.clear();
}

/**
 * @brief Cp2k::run The running entry for the calculation
 * @param directory The path to the directory where the calculation
 *  is running.
 * The running of the calculation is actually controlled
 * by the JobScheduler.
 */
void Cp2k::run(const std::string& directory) {
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

} // namespace atomsciflow
