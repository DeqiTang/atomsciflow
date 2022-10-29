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

/// @file src/atomsciflow/qe/pwscf.cpp
/// @author Deqi Tang
/// Mail: deqi_tang@163.com
/// Created Time: Tue 02 Feb 2021 05:56:20 PM CST

#include "atomsciflow/qe/pwscf.h"

#include <regex>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "atomsciflow/utils/structure.h"
#include "atomsciflow/base/element.h"
#include "atomsciflow/server/submit_script.h"
#include "atomsciflow/remote/server.h"

namespace atomsciflow::qe {

namespace fs = boost::filesystem;

PwScf::PwScf() {
    this->namelists["control"] = qe::gen::control();
    this->namelists["system"] = qe::gen::system();
    this->namelists["electrons"] = qe::gen::electrons();
    this->namelists["ions"] = qe::gen::ions();
    this->namelists["cell"] = qe::gen::cell();

    this->set_param("control", "pseudo_dir", "./");

    set_card_option("k_points", "gamma");
    set_card_data("k_points(gamma)", std::vector<int>{}, 0);
    set_card_data("k_points(automatic)", std::vector<int>{1, 1, 1, 0, 0, 0}, 0);
    
    job.set_run("input", "pw.in");
    job.set_run("output", "pw.out");
    job.set_run("cmd", "$ASF_CMD_QE_PWSCF");
    job.set_run("script_name_head", "pwscf-run");
}

PwScf::~PwScf() {

}

std::string PwScf::to_string() {
    return this->to_string("");    
}

std::string PwScf::to_string(std::string indent) {
    std::string out = "";

    std::vector<std::string> namelists_order;
    namelists_order.push_back("control");
    namelists_order.push_back("system");
    namelists_order.push_back("electrons");
    namelists_order.push_back("ions");
    namelists_order.push_back("cell");
    for (const auto& item : namelists_order) {
        if (false == this->namelists[item].status) {
            continue;
        } else {
            out += this->namelists[item].to_string(indent);
            out += "\n";
        }
    }
    std::vector<std::string> cards_order;
    cards_order.push_back("atomic_species");
    cards_order.push_back("atomic_positions");
    cards_order.push_back("k_points");
    cards_order.push_back("additional_k_pionts");
    cards_order.push_back("cell_parameters");
    cards_order.push_back("constraints");
    cards_order.push_back("occupations");
    cards_order.push_back("atomic_velocities");
    cards_order.push_back("atomic_forces");
    cards_order.push_back("solvents");
    cards_order.push_back("hubbard");
    for (auto& item : cards_order) {
        if (cards.find(item) == cards.end()) {
            continue;
        }
        out += cards[item]->to_string();
    }
    out += "\n";

    return out;
}

void PwScf::get_xyz(const std::string& xyzfile) {
    job.set_run("xyz_file", fs::absolute(xyzfile).string());

    this->xyz.read_xyz_file(xyzfile);
    this->set_card_option("cell_parameters", "angstrom");
    this->set_card_data("cell_parameters", this->xyz.cell);
    this->set_card_option("atomic_positions", "crystal");
    std::vector<Atom> atoms_frac;
    atoms_cart_to_frac(atoms_frac, this->xyz.atoms, this->xyz.cell);
    for (int k = 0; k < atoms_frac.size(); k++) {
        this->set_card_data("atomic_positions", atoms_frac[k].name, k, 0);
        this->set_card_data("atomic_positions", (boost::format("%1$10.6f")%atoms_frac[k].x).str(), k, 1);
        this->set_card_data("atomic_positions", (boost::format("%1$10.6f")%atoms_frac[k].y).str(), k, 2);
        this->set_card_data("atomic_positions", (boost::format("%1$10.6f")%atoms_frac[k].z).str(), k, 3);
    }

    auto element_map = get_element_number_map();

    std::vector<fs::path> all_files;
    for (const auto& p : fs::directory_iterator(fs::path(config.get_pseudo_pot_dir()["qe"]) / "SSSP_efficiency_pseudos")) {
        all_files.emplace_back(p.path());
    }
    int i = 0;
    for (const auto& element : xyz.elements_set) {
        set_card_data("atomic_species", element, i, 0);
        set_card_data("atomic_species", boost::lexical_cast<std::string>(element_map[element].mass), i, 1);

        std::string pat_string = "";
        if (element.size() == 1) {
            pat_string = (boost::format("^[%1%|%2%][.|_]")
                % boost::to_upper_copy(element)
                % boost::to_lower_copy(element)
            ).str();
        } else {
            pat_string = (boost::format("^[%1%|%2%][%3%|%4%][.|_]")
                % boost::to_upper_copy(boost::lexical_cast<std::string>(element[0]))
                % boost::to_lower_copy(boost::lexical_cast<std::string>(element[0]))
                % boost::to_upper_copy(boost::lexical_cast<std::string>(element[1]))
                % boost::to_lower_copy(boost::lexical_cast<std::string>(element[1]))
            ).str();            
        }
        for (const auto& item : all_files) {
            std::regex pat{pat_string};
            if (std::regex_search(item.filename().string(), pat)) {
                set_card_data("atomic_species", item.filename().string(), i, 2);
                break;
            }
        }
        i++;
    }

    set_param("system", "ibrav", 0);
    set_param("system", "nat", boost::lexical_cast<int>(xyz.atoms.size()));
    set_param("system", "ntyp", xyz.nspecies);
    set_param("system", "ecutwfc", 100);
    set_param("system", "input_dft", "PBE");
}

void PwScf::set_param(const std::string& namelist, std::string key, int value) {
    this->namelists[namelist].set_param(key, value);
}

void PwScf::set_param(const std::string& namelist, std::string key, double value) {
    this->namelists[namelist].set_param(key, value);
}

void PwScf::set_param(const std::string& namelist, std::string key, std::string value) {
    this->namelists[namelist].set_param(key, value);
}

void PwScf::set_param(const std::string& namelist, std::string key, std::vector<int> value) {
    this->namelists[namelist].set_param(key, value);
}

void PwScf::set_param(const std::string& namelist, std::string key, std::vector<double> value) {
    this->namelists[namelist].set_param(key, value);
}

void PwScf::set_param(const std::string& namelist, std::string key, std::vector<std::string> value) {
    this->namelists[namelist].set_param(key, value);
}

void PwScf::set_param(const std::string& namelist, std::string key, std::vector<std::vector<int>> value) {
    this->namelists[namelist].set_param(key, value);
}

void PwScf::set_param(const std::string& namelist, std::string key, std::vector<std::vector<double>> value) {
    this->namelists[namelist].set_param(key, value);
}

void PwScf::set_param(const std::string& namelist, std::string key, std::vector<std::vector<std::string>> value) {
    this->namelists[namelist].set_param(key, value);
}

void PwScf::basic_setting_ions(std::string calc = "relax") {
    if ("relax" == calc) {
        this->namelists["ions"].set_param("ion_dynamics", "bfgs");
    } else if ("md" == calc) {
        this->namelists["ions"].set_param("ion_dynamics", "verlet");
        this->namelists["ions"].set_param("ion_temperature", "not_controlled");
        this->namelists["ions"].set_param("tempw", 300);
    } else if ("vc-relax" == calc) {
        this->namelists["ions"].set_param("ion_dynamics", "bfgs");
    }
}

void PwScf::basic_setting_electrons() {
    this->namelists["electrons"].set_param("conv_thr", 1.0E-6);
    // plain mixing mode means Broyden mixing scheme
    // for charge density
    this->namelists["electrons"].set_param("mixing_mode", "plain");
    // the mixing factor for scf iteration
    this->namelists["electrons"].set_param("mixing_beta", 0.7E0);
    // number of previous iterations used in the mixing
    // procedure. More memoery is needed for larger mixing_ndim
    // if memoery is in shortage, reduce from 8 to 4.
    this->namelists["electrons"].set_param("mixing_ndim", 8);
    this->namelists["electrons"].set_param("diagonalization", "david");
}

void PwScf::basic_setting_control(std::string calc = "scf") {
    this->namelists["control"].set_param("calculation", calc);
    this->namelists["control"].set_param("prefix", "pwscf");
    if ("scf" == calc) {
        this->namelists["control"].set_param("outdir", "./tmp");
        this->namelists["control"].set_param("pseudo_dir", "./");
        this->namelists["control"].set_param("wf_collect", ".true.");
    } else if ("nscf" == calc) {
        this->namelists["control"].set_param("outdir", "./tmp");
        this->namelists["control"].set_param("pseudo_dir", "./tmp");
        this->namelists["control"].set_param("wf_collect", ".true.");
    } else if ("bands" == calc) {
        this->namelists["control"].set_param("outdir", "./tmp");
        this->namelists["control"].set_param("pseudo_dir", "./");
        this->namelists["control"].set_param("wf_collect", ".true.");
    } else if ("relax" == calc) {
        this->namelists["control"].set_param("outdir", "./tmp");
        this->namelists["control"].set_param("pseudo_dir", "./");
        this->namelists["control"].set_param("wf_collect", ".true.");
    } else if (calc == "md") {
        this->namelists["control"].set_param("outdir", "./tmp");
        this->namelists["control"].set_param("pseudo_dir", "./");
        this->namelists["control"].set_param("wf_collect", ".true.");
    } else if ("vc-relax" == calc) {
        this->namelists["control"].set_param("outdir", "./tmp");
        this->namelists["control"].set_param("pseudo_dir", "./");
        this->namelists["control"].set_param("wf_collect", ".true.");
    } else if ("vc-md" == calc) {
        this->namelists["control"].set_param("outdir", "./tmp");
        this->namelists["control"].set_param("pseudo_dir", "./");
        this->namelists["control"].set_param("wf_collect", ".true.");
    }
}

void PwScf::set_occupations(std::string occupations = "smearing", std::string smearing = "gaussian", double degauss=0.001) {
    this->namelists["system"].set_param("occupations", occupations);
    if ("smearing" == occupations) {
        this->namelists["system"].set_param("smearing", smearing);
        this->namelists["system"].set_param("degauss", degauss);
    }
    if ("tetrahedra" == occupations) {
        this->namelists["system"].set_param("smearing", "");
        this->namelists["system"].set_param("degauss", "");
    }
    if ("tetrahedra_lin" == occupations) {
        this->namelists["system"].set_param("smearing", "");
        this->namelists["system"].set_param("degauss", "");
    }
    if ("tetrahedra_opt" == occupations) {
        this->namelists["system"].set_param("smearing", "");
        this->namelists["system"].set_param("degauss", "");
    }
    if ("fixed" == occupations) {
        this->namelists["system"].set_param("smearing", "");
        this->namelists["system"].set_param("degauss", "");
    }
    if ("from_input" == occupations) {
        this->namelists["system"].set_param("smearing", "");
        this->namelists["system"].set_param("degauss", "");
    }
}

void PwScf::run(const std::string& directory) {
    job.steps.clear();
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << boost::format("cat > %1%<<EOF\n") % job.run_params["input"];
    step << this->to_string();
    step << "EOF\n";
    for (const auto& item : this->xyz.elements_set) {
        step << "# pseudopotential file for element: " << item << "\n";
        if (job.run_params["qe_pseudo_choice"] == "sssp-efficiency") {
            step << boost::format("for item in %1%/*\n") 
                % (fs::path(config.get_pseudo_pot_dir()["qe"]) / "SSSP_efficiency_pseudos").string();
        } else if (job.run_params["qe_pseudo_choice"] == "sssp-precision") {
            step << boost::format("for item in %1%/*\n") 
                % (fs::path(config.get_pseudo_pot_dir()["qe"]) / "SSSP_precision_pseudos").string();            
        } else {
            std::cout << "atomsciflow::PwScf::run\n";
            std::cout << "you have to specify the right qe_pseudo_choice\n";
            std::exit(0);
        }
        step << "do\n";
        if (item.size() == 1) {
            step << boost::format("if [[ ${item} =~ /[%1%|%2%][.|_] ]]\n")
                % boost::to_upper_copy(item)
                % boost::to_lower_copy(item);
        } else {
            step << boost::format("if [[ ${item} =~ /[%1%|%2%][%3%|%4%][.|_] ]]\n")
                % boost::to_upper_copy(boost::lexical_cast<std::string>(item[0]))
                % boost::to_lower_copy(boost::lexical_cast<std::string>(item[0]))
                % boost::to_upper_copy(boost::lexical_cast<std::string>(item[1]))
                % boost::to_lower_copy(boost::lexical_cast<std::string>(item[1]));
        }
        step << "then\n";
        step << "cp ${item} ${ABSOLUTE_WORK_DIR}/\n";
        step << "fi\n";
        step << "done\n";
    }
    step << boost::format("$CMD_HEAD %1% < %2% | tee %3%\n") % job.run_params["cmd"] % job.run_params["input"] % job.run_params["output"];
    job.steps.push_back(step.str());
    step.clear();

    job.run(directory);
}

} // namespcae atomsciflow::qe
