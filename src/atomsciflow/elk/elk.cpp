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

/// @file src/atomsciflow/elk/elk.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Wed 02 Mar 2022 02:39:17 PM CST

#include "atomsciflow/elk/elk.h"

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include "atomsciflow/base/element.h"
#include "atomsciflow/server/submit_script.h"
#include "atomsciflow/remote/server.h"
#include "atomsciflow/utils/structure.h"

namespace atomsciflow::elk {

namespace fs = boost::filesystem;

Elk::Elk() {

    this->set_param("tasks", -11);
    this->set_param("ngridk", std::vector<int>{1, 1, 1});
    this->set_param("sppath", "\"${ASF_CMD_ELK_SPPATH}/\"");

    job.set_run("cmd", "$ASF_CMD_ELK");
    job.set_run("script_name_head", "elk-run");
}

Elk::~Elk() {
}

std::string Elk::to_string() {
    std::ostringstream out;

    for (const auto& item : this->blocks) {
        if (item.second->status == false) {
            continue;
        }
        if (item.first == "atoms") {
            out << "atoms\n";
            out << boost::format("  %1% : nspecies\n") % item.second->params["nspecies"].value[0][0];
            for (int i = 0; i < boost::lexical_cast<int>(item.second->params["nspecies"].value[0][0]); i++) {
                out << boost::format("  \'%1%\' : spfname(%2%)\n")
                    % item.second->params[(boost::format("spfname(%1%)")%i).str()].value[0][0]
                    % i;
                out << boost::format("  %1% : natoms(%2%)\n")
                    % item.second->params[(boost::format("natoms(%1%)")%i).str()].value[0][0]
                    % i;
                for (int j = 0; j < boost::lexical_cast<int>(item.second->params[(boost::format("natoms(%1%)")%i).str()].value[0][0]); j++) {
                    if (item.second->params.find((boost::format("bfcmt(%1%,%2%)")%j%i).str()) == item.second->params.end()) {
                        out << boost::format("  %1$10.8f %2$10.8f %3$10.8f : atposl(%4%,%5%)\n")
                            % boost::lexical_cast<double>(item.second->params[(boost::format("atposl(%1%,%2%)")%j%i).str()].value[0][0])
                            % boost::lexical_cast<double>(item.second->params[(boost::format("atposl(%1%,%2%)")%j%i).str()].value[0][1])
                            % boost::lexical_cast<double>(item.second->params[(boost::format("atposl(%1%,%2%)")%j%i).str()].value[0][2])
                            % j % i;
                    } else {
                        out << boost::format("  %1$10.8f %2$10.8f %3$10.8f %4$10.8f %5$10.8f %6$10.8f : atposl(%7%,%8%), bfcmt(%7%,%8%)\n")
                            % boost::lexical_cast<double>(item.second->params[(boost::format("atposl(%1%,%2%)")%j%i).str()].value[0][0])
                            % boost::lexical_cast<double>(item.second->params[(boost::format("atposl(%1%,%2%)")%j%i).str()].value[0][1])
                            % boost::lexical_cast<double>(item.second->params[(boost::format("atposl(%1%,%2%)")%j%i).str()].value[0][2])
                            % boost::lexical_cast<double>(item.second->params[(boost::format("bfcmt(%1%,%2%)")%j%i).str()].value[0][0])
                            % boost::lexical_cast<double>(item.second->params[(boost::format("bfcmt(%1%,%2%)")%j%i).str()].value[0][1])
                            % boost::lexical_cast<double>(item.second->params[(boost::format("bfcmt(%1%,%2%)")%j%i).str()].value[0][2])
                            % j % i;
                    }
                }
            }
            out << "\n";
        } else if (item.first == "dft+u") {
            out << "dft+u\n";
            out << boost::format("  %1% %2% : dftu, inpdftu\n")
                % item.second->params["dftu"].value[0][0]
                % item.second->params["inpdftu"].value[0][0];
            out << boost::format("  %1% %2% %3$10.8f %4$10.8f : is, l, u, j\n")
                % item.second->params["is"].value[0][0]
                % item.second->params["l"].value[0][0]
                % boost::lexical_cast<double>(item.second->params["u"].value[0][0])
                % boost::lexical_cast<double>(item.second->params["j"].value[0][0]);
            out << "\n";            
        } else if (item.first == "plot1d") {
            out << "plot1d\n";
            out << boost::format("  %1% %2% : nvp1d, npp1d\n")
                % item.second->params["nvp1d"].value[0][0]
                % item.second->params["npp1d"].value[0][0];
            for (int i = 0; i < boost::lexical_cast<int>(item.second->params["nvp1d"].value[0][0]); i++) {
                out << boost::format("  %1$10.8f %2$10.8f %3$10.8f : %4%\n")
                    % boost::lexical_cast<double>(item.second->params[(boost::format("vvlp1d(%1%)")%i).str()].value[0][0])
                    % boost::lexical_cast<double>(item.second->params[(boost::format("vvlp1d(%1%)")%i).str()].value[0][1])
                    % boost::lexical_cast<double>(item.second->params[(boost::format("vvlp1d(%1%)")%i).str()].value[0][2])
                    % (boost::format("vvlp1d(%1%)")%i).str();
            }
            out << "\n";
        } else if (item.first == "plot2d") {
            out << "plot2d\n";
            for (int i = 0; i < 3; i++) {
                out << boost::format("  %1$10.8f %2$10.8f %3$10.8f : vclp2d(%4%)\n")
                    % boost::lexical_cast<double>(item.second->params[(boost::format("vclp2d(%1%)") % i).str()].value[0][0])
                    % boost::lexical_cast<double>(item.second->params[(boost::format("vclp2d(%1%)") % i).str()].value[0][1])
                    % boost::lexical_cast<double>(item.second->params[(boost::format("vclp2d(%1%)") % i).str()].value[0][2])
                    % i;
            }
            out << boost::format("  %1% %2% : np2d\n")
                % item.second->params["np2d"].value[0][0]
                % item.second->params["np2d"].value[0][1];
            out << "\n";
        } else if (item.first == "plot3d") {
            out << "plot3d\n";
            for (int i = 0; i < 4; i++) {
                out << boost::format("  %1$10.8f %2$10.8f %3$10.8f : vclp3d(%4%)\n") 
                    % boost::lexical_cast<double>(item.second->params[(boost::format("vclp3d(%1%)") % i).str()].value[0][0])
                    % boost::lexical_cast<double>(item.second->params[(boost::format("vclp3d(%1%)") % i).str()].value[0][1])
                    % boost::lexical_cast<double>(item.second->params[(boost::format("vclp3d(%1%)") % i).str()].value[0][2])
                    % i;
            }
            out << boost::format("  %1% %2% %3%: np3d\n")
                % item.second->params["np3d"].value[0][0]
                % item.second->params["np3d"].value[0][1]
                % item.second->params["np3d"].value[0][2];    
            out << "\n";
        } else if (item.first == "mommtfix") {
            out << "mommtfix\n";
            out << boost::format("  %1% %2% %3$10.8f %4$10.8f %5$10.8f : is, ia, mommtfix\n")
                % item.second->params["is"].value[0][0]
                % item.second->params["ia"].value[0][0]
                % boost::lexical_cast<double>(item.second->params["mommtfix"].value[0][0])
                % boost::lexical_cast<double>(item.second->params["mommtfix"].value[0][1])
                % boost::lexical_cast<double>(item.second->params["mommtfix"].value[0][2]);
            out << "\n";
        } else if (item.first == "phwrite") {
            out << "phwrite\n";
            out << boost::format("  %1% : nphwrt\n") % item.second->params["nphwrt"].value[0][0];
            for (int i = 0; i < boost::lexical_cast<int>(item.second->params["nphwrt"].value[0][0]); i++) {
                out << boost::format("  %1$10.8f %2$10.8f %3$10.8f : vqlwrt(%4%)\n")
                    % boost::lexical_cast<double>(item.second->params[(boost::format("vqlwrt(%1%)")%i).str()].value[0][0])
                    % boost::lexical_cast<double>(item.second->params[(boost::format("vqlwrt(%1%)")%i).str()].value[0][1])
                    % boost::lexical_cast<double>(item.second->params[(boost::format("vqlwrt(%1%)")%i).str()].value[0][2])
                    % i;                    
            }
            out << "\n";
        } else if (item.first == "pulse") {
            out << "pulse\n";
            out << boost::format("  %1% : n\n") % item.second->params["n"].value[0][0];
            for (int i = 0; i < boost::lexical_cast<int>(item.second->params["n"].value[0][0]); i++) {
                out << boost::format("  %1$10.8f %2$10.8f %3$10.8f %4$10.8f %5$10.8f %6$10.8f %7$10.8f %8$10.8f : a0(%9%), w(%9%), phi(%9%), rc(%9%), t0(%9%), d(%9%)\n")
                    % boost::lexical_cast<double>(item.second->params[(boost::format("a0(%1%)")%i).str()].value[0][0])
                    % boost::lexical_cast<double>(item.second->params[(boost::format("a0(%1%)")%i).str()].value[0][1])
                    % boost::lexical_cast<double>(item.second->params[(boost::format("a0(%1%)")%i).str()].value[0][2])
                    % boost::lexical_cast<double>(item.second->params[(boost::format("w(%1%)")%i).str()].value[0][0])
                    % boost::lexical_cast<double>(item.second->params[(boost::format("phi(%1%)")%i).str()].value[0][0])
                    % boost::lexical_cast<double>(item.second->params[(boost::format("rc(%1%)")%i).str()].value[0][0])
                    % boost::lexical_cast<double>(item.second->params[(boost::format("t0(%1%)")%i).str()].value[0][0])
                    % boost::lexical_cast<double>(item.second->params[(boost::format("d(%1%)")%i).str()].value[0][0])
                    % i;
            }
            out << "\n";
        } else if (item.first == "ramp") {
            out << "ramp\n";
            out << boost::format("  %1% : n\n") % item.second->params["n"].value[0][0];
            for (int i = 0; i < boost::lexical_cast<int>(item.second->params["n"].value[0][0]); i++) {
                out << boost::format("  %1$10.8f %2$10.8f %3$10.8f %4$10.8f %5$10.8f %6$10.8f : a0(%7%), t0(%7%), c1(%7%), c2(%7%)\n")
                    % boost::lexical_cast<double>(item.second->params[(boost::format("a0(%1%)")%i).str()].value[0][0])
                    % boost::lexical_cast<double>(item.second->params[(boost::format("a0(%1%)")%i).str()].value[0][1])
                    % boost::lexical_cast<double>(item.second->params[(boost::format("a0(%1%)")%i).str()].value[0][2])
                    % boost::lexical_cast<double>(item.second->params[(boost::format("t0(%1%)")%i).str()].value[0][0])
                    % boost::lexical_cast<double>(item.second->params[(boost::format("c1(%1%)")%i).str()].value[0][0])
                    % boost::lexical_cast<double>(item.second->params[(boost::format("c2(%1%)")%i).str()].value[0][0])
                    % i;
            }
            out << "\n";
        } else if (item.first == "tasks") {
            out << "tasks\n";
            for (const auto& task : item.second->params["tasks"].value[0]) {
                out << "  " << task << "\n";
            }
            out << "\n";
        } else if (item.first == "tm3fix") {
            out << "tm3fix\n";
            out << boost::format("  %1% : ntmfix\n") % item.second->params["ntmfix"].value[0][0];
            for (int i = 0; i < boost::lexical_cast<int>(item.second->params["ntmfix"].value[0][0]); i++) {
                out << boost::format("  %1% %2% %3% : is(%4%), ia(%4%), l(%4%)\n")
                    % item.second->params[(boost::format("is(%1%)")%i).str()].value[0][0]
                    % item.second->params[(boost::format("ia(%1%)")%i).str()].value[0][0]
                    % item.second->params[(boost::format("l(%1%)")%i).str()].value[0][0]
                    % i;
                out << boost::format("  %1% %2% %3% %4% : k(%5%), p(%5%), r(%5%), t(%5%)\n")
                    % item.second->params[(boost::format("k(%1%)")%i).str()].value[0][0]
                    % item.second->params[(boost::format("p(%1%)")%i).str()].value[0][0]
                    % item.second->params[(boost::format("r(%1%)")%i).str()].value[0][0]
                    % item.second->params[(boost::format("t(%1%)")%i).str()].value[0][0]
                    % i;
                out << boost::format("  %1$10.8f : wkpr(t)(%2%)\n")
                    % boost::lexical_cast<double>(item.second->params[(boost::format("wkprt(t)(%1%)")%i).str()].value[0][0])
                    % i;
            }
            out << "\n";
        } else if (item.first == "wplot") {
            out << "wplot\n";
            out << boost::format("  %1% %2% %3% : nwplot, ngrkf, nswplot\n")
                % item.second->params["nwplot"].value[0][0]
                % item.second->params["ngrkf"].value[0][0]
                % item.second->params["nswplot"].value[0][0];
            out << boost::format("  %1$10.8f %2$10.8f : wplot\n")
                % boost::lexical_cast<double>(item.second->params["wplot"].value[0][0])
                % boost::lexical_cast<double>(item.second->params["wplot"].value[0][1]);
            out << "\n";        
        } else if (item.first == "xctype") {
            out << "xctype\n";
            for (const auto& val : item.second->params["xctype"].value[0]) {
                out << "  " << val;
            }
            // out << " : xctype\n";
            // for xctype, " : xctype" may arouse error. so we do not write it
            out << "\n"; 
            out << "\n";
        } else {
            out << item.second->to_string();
        }
    }

    return out.str();
}

void Elk::get_xyz(std::string xyzfile) {
    this->xyz.read_xyz_file(xyzfile);
    job.set_run("xyz_file", fs::absolute(xyzfile).string());

    double bohr_to_angstrom = 0.529177249;
    std::vector<std::vector<double>> cell_bohr;
    for (int i = 0; i < 3; ++i) {
        std::vector<double> vec;
        for (int j = 0; j < 3; ++j) {
            vec.push_back(this->xyz.cell[i][j] / bohr_to_angstrom);
        }
        cell_bohr.push_back(vec);
    }
    this->set_param("scale", 1);
    // this->set_param("avec/avec", cell_bohr);
    this->set_param("avec/avec(0)", cell_bohr[0]);
    this->set_param("avec/avec(1)", cell_bohr[1]);
    this->set_param("avec/avec(2)", cell_bohr[2]);

    this->set_param("atoms/nspecies", int(this->xyz.elements_set.size()));

    std::map<std::string, int> elem_natom;
    for (const auto& element : this->xyz.elements_set) {
        elem_natom[element] = 0;
        for (const auto& atom : this->xyz.atoms) {
            if (element == atom.name) {
                elem_natom[element] += 1;
            }
        }
    }
    
    std::vector<Atom> atoms_frac;
    atoms_cart_to_frac(atoms_frac, this->xyz.atoms, this->xyz.cell);
    int i = 0;
    for (const auto& item : elem_natom) {
        set_param((boost::format("atoms/spfname(%1%)")%i).str(), item.first+".in");
        set_param((boost::format("atoms/natoms(%1%)")%i).str(), item.second);
        int j = 0;
        for (const auto& atom : atoms_frac) {
            if (boost::to_lower_copy(atom.name) == boost::to_lower_copy(item.first)) {
                std::vector<double> pos;
                pos.push_back(atom.x);
                pos.push_back(atom.y);
                pos.push_back(atom.z);
                set_param((boost::format("atoms/atposl(%1%,%2%)")%j%i).str(), pos);
                j++;
            }
        }
        i++;
    }
}

template <typename T>
void Elk::set_param(const std::string& path, T value) {
    std::vector<std::string> str_vec;
    boost::split(str_vec, path, boost::is_any_of("/"), boost::token_compress_on);

    if (this->blocks.find(str_vec[0]) == this->blocks.end()) {
        this->blocks[str_vec[0]] = std::make_shared<Block>();
        this->blocks[str_vec[0]]->name = str_vec[0];
    }

    if (boost::contains(path, "/")) {
        this->blocks[str_vec[0]]->set_param(str_vec[1], value);
    } else {
        this->blocks[str_vec[0]]->set_param(str_vec[0], value);
    }
}

void Elk::py_set_param(const std::string& path, int value) {
    this->set_param(path, value);
}

void Elk::py_set_param(const std::string& path, double value) {
    this->set_param(path, value);
}

void Elk::py_set_param(const std::string& path, std::string value) {
    this->set_param(path, value);
}

void Elk::py_set_param(const std::string& path, std::vector<int> value) {
    this->set_param(path, value);
}

void Elk::py_set_param(const std::string& path, std::vector<double> value) {
    this->set_param(path, value);
}

void Elk::py_set_param(const std::string& path, std::vector<std::string> value) {
    this->set_param(path, value);
}

void Elk::py_set_param(const std::string& path, std::vector<std::vector<int>> value) {
    this->set_param(path, value);
}

void Elk::py_set_param(const std::string& path, std::vector<std::vector<double>> value) {
    this->set_param(path, value);
}

void Elk::py_set_param(const std::string& path, std::vector<std::vector<std::string>> value) {
    this->set_param(path, value);
}

void Elk::set_kpath(const Kpath& kpath) {
    set_param("plot1d/nvp1d", int(kpath.labels.size()));
    set_param("plot1d/npp1d", 100);
    for (int i = 0; i < kpath.labels.size(); i++) {
        set_param(
            (boost::format("plot1d/vvlp1d(%1%)")%i).str(), 
            kpath.coords[i]
        );
    }
}

/// @param directory a place for all the generated files
void Elk::run(const std::string& directory) {
    job.steps.clear();
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << "cat > elk.in<<EOF\n";
    step << this->to_string();
    step << "EOF\n";

    step << boost::format("$CMD_HEAD %1% > elk.out\n") % this->job.run_params["cmd"];
    job.steps.push_back(step.str());
    step.clear();

    job.run(directory);
}

// explicit template instantiation
template void Elk::set_param<int>(const std::string& path, int);
template void Elk::set_param<double>(const std::string& path, double);
template void Elk::set_param<std::string>(const std::string& path, std::string);
template void Elk::set_param<std::vector<int>>(const std::string& path, std::vector<int>);
template void Elk::set_param<std::vector<double>>(const std::string& path, std::vector<double>);
template void Elk::set_param<std::vector<std::string>>(const std::string& path, std::vector<std::string>);
template void Elk::set_param<std::vector<std::vector<int>>>(const std::string& path, std::vector<std::vector<int>>);
template void Elk::set_param<std::vector<std::vector<double>>>(const std::string& path, std::vector<std::vector<double>>);
template void Elk::set_param<std::vector<std::vector<std::string>>>(const std::string& path, std::vector<std::vector<std::string>>);

} // namespace atomsciflow::elk
