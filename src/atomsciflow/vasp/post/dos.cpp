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

#include "atomsciflow/vasp/post/dos.h"

#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <armadillo>
#include <iostream>

namespace atomsciflow::vasp::post {

namespace fs = boost::filesystem;

Dos::Dos() {
    this->set_run("program-out", "OUTCAR");
    this->set_run("output-json", "post-dos.json");
}

void Dos::run(const std::string& directory) {
    Post::run(directory);

    std::ofstream out;

    pt::ptree vasprun;
    pt::read_xml((fs::path(directory) / "vasprun.nscf.dos.xml").string(), vasprun);

    double fermi_energy = vasprun.get<double>("modeling.calculation.dos.i");

    std::vector<std::string> str_vec;
    std::vector<double> energies_vec;
    for (auto& item : vasprun.get_child("modeling.calculation.dos.total.array.set.set")) {
        if (item.first == "<xmlattr>") {
            continue;
        }
        boost::split(str_vec, item.second.data(), boost::is_any_of(" "), boost::token_compress_on);
        energies_vec.push_back(boost::lexical_cast<double>(str_vec[1]));
    }
    arma::rowvec energies = arma::conv_to<arma::rowvec>::from(energies_vec);

    std::map<std::string, arma::rowvec> total_dos;
    std::map<std::string, arma::rowvec> total_integrated_dos;
    for (auto& spin : vasprun.get_child("modeling.calculation.dos.total.array.set")) {
        if (spin.first == "<xmlattr>") {
            continue;
        }
        std::string key = boost::replace_all_copy(spin.second.get<std::string>("<xmlattr>.comment"), " ", "-");
        total_dos[key] = arma::conv_to<arma::rowvec>::from(arma::zeros(energies.n_cols));
        total_integrated_dos[key] = arma::conv_to<arma::rowvec>::from(arma::zeros(energies.n_cols));
        int row_index = 0;
        for (auto& row : spin.second) {
            if (row.first == "<xmlattr>") {
                continue;
            }
            boost::split(str_vec, row.second.data(), boost::is_any_of(" "), boost::token_compress_on);
            total_dos[key].at(row_index) = boost::lexical_cast<double>(str_vec[2]);
            total_integrated_dos[key].at(row_index) = boost::lexical_cast<double>(str_vec[3]);
            row_index++;
        }
    }

    for (auto& item : total_dos) {
        out.open((fs::path(directory) / "post.dir" / (boost::format("total-%1%.data")%item.first).str()).string());
        for (int i = 0; i < energies.size(); i++) {
            out << boost::format("%1$10.8f %2$10.8f\n") % energies.at(i) % item.second.at(i);
        }
        out.close();
    }

    for (auto& item : total_integrated_dos) {
        out.open((fs::path(directory) / "post.dir" / (boost::format("total-integrated-%1%.data")%item.first).str()).string());
        for (int i = 0; i < energies.size(); i++) {
            out << boost::format("%1$10.8f %2$10.8f\n") % energies.at(i) % item.second.at(i);
        }
        out.close();
    }

    std::map<std::string, arma::rowvec> ion_orbital_proj;
    std::vector<std::string> fields;
    for (auto& item : vasprun.get_child("modeling.calculation.dos.partial.array")) {
        if (item.first == "field") {
            fields.push_back(boost::replace_all_copy(item.second.data(), " ", ""));
        }
    }
    for (auto& ion : vasprun.get_child("modeling.calculation.dos.partial.array.set")) {
        if (ion.first == "<xmlattr>") {
            continue;
        }
        for (auto& spin : ion.second) {
            if (spin.first == "<xmlattr>") {
                continue;
            }
            // ignoring i = 0 which is energy
            for (int i = 1; i < fields.size(); i++) {
                std::string key = (boost::format("%1%-%2%-%3%")
                    % boost::replace_all_copy(ion.second.get<std::string>("<xmlattr>.comment"), " ", "")
                    % fields[i]
                    % boost::replace_all_copy(spin.second.get<std::string>("<xmlattr>.comment"), " ", "-")
                ).str();
                if (ion_orbital_proj.find(key) == ion_orbital_proj.end()) {
                    // remember to initialize the rowvec with 0
                    ion_orbital_proj[key] = arma::conv_to<arma::rowvec>::from(arma::zeros(energies.n_cols));
                }
            }
            int row_index = 0;
            for (auto& row : spin.second) {
                if (row.first == "<xmlattr>") {
                    continue;
                }
                boost::split(str_vec, row.second.data(), boost::is_any_of(" "), boost::token_compress_on);
                for (int i = 1; i < fields.size(); i++) {
                    std::string key = (boost::format("%1%-%2%-%3%")
                        % boost::replace_all_copy(ion.second.get<std::string>("<xmlattr>.comment"), " ", "")
                        % fields[i]
                        % boost::replace_all_copy(spin.second.get<std::string>("<xmlattr>.comment"), " ", "-")
                    ).str();
                    ion_orbital_proj[key].at(row_index) += boost::lexical_cast<double>(str_vec[i+1]);
                }
                row_index++;
            }
        }
    }


    for (auto& item : ion_orbital_proj) {
        out.open((fs::path(directory) / "post.dir" / (boost::format("ion-orbital-proj-%1%.data")%item.first).str()).string());
        for (int i = 0; i < energies.size(); i++) {
            out << boost::format("%1$10.8f %2$10.8f\n") % energies.at(i) % item.second.at(i);
        }
        out.close();
    }

    out.open((fs::path(directory) / "post.dir/dos.gnuplot").string());
    out << "set terminal png\n";
    out << "set parametric\n";
    out << "set title 'Density of state' font ',15'\n";
    out << "set ylabel 'Density of state' font ',15'\n";
    out << "set xlabel 'Energy (eV)' font ',15'\n";
    out << "set ytics font ',15'\n";
    out << "set xtics font ',15'\n";
    out << "set border linewidth 3\n";
    out << "set autoscale\n";
    out << "set xrange [-10:10]\n";

    out << "set arrow from 0, graph 0 to 0, graph 1 nohead linecolor rgb \'black\' linewidth 0.5\n";
    out << "set arrow from -10, 0 to 10, 0 nohead linecolor rgb \'black\' linewidth 0.5\n";

    int i = 0;

    out << "set output \'total-dos.png\'\n";
    i = 0;
    out << "plot \\\n";
    for (auto item : total_dos) {
        if (i != total_dos.size() -1) {
            out << boost::format("  \'total-%1%.data\' using (column(1)-(%2%)):(column(2)*(%3%)) w l title \'%4%\' linewidth 3,\\\n")
                % item.first
                % fermi_energy
                % (boost::contains(item.first, "spin-2") ? -1 : 1)
                % item.first;
        } else {
            out << boost::format("  \'total-%1%.data\' using (column(1)-(%2%)):(column(2)*(%3%)) w l title \'%4%\' linewidth 3\n")
                % item.first
                % fermi_energy
                % (boost::contains(item.first, "spin-2") ? -1 : 1)
                % item.first;
        }
        i++;
    }

    out << "set output \'total-and-integrated-dos.png\'\n";
    out << "plot \\\n";
    for (auto& item : total_dos) {   
        out << boost::format("  \'total-%1%.data\' using (column(1)-(%2%)):(column(2)*(%3%)) w l title \'%4%\' linewidth 3,\\\n")
            % item.first
            % fermi_energy
            % (boost::contains(item.first, "spin-2") ? -1 : 1)
            % item.first;
    }
    i = 0;
    for (auto item : total_integrated_dos) {
        if (i != total_dos.size() -1) {
            out << boost::format("  \'total-integrated-%1%.data\' using (column(1)-(%2%)):(column(2)*(%3%)) w l title \'%4%\' linewidth 3,\\\n")
                % item.first
                % fermi_energy
                % (boost::contains(item.first, "spin-2") ? -1 : 1)
                % item.first;
        } else {
            out << boost::format("  \'total-integrated-%1%.data\' using (column(1)-(%2%)):(column(2)*(%3%)) w l title \'%4%\' linewidth 3\n")
                % item.first
                % fermi_energy
                % (boost::contains(item.first, "spin-2") ? -1 : 1)
                % item.first;
        }
        i++;
    }

    out << "set output \'ion-orbital-proj.png\'\n";
    out << "plot \\\n";        
    i = 0;
    for (auto& item : ion_orbital_proj) {
        if (i != ion_orbital_proj.size() - 1) {
            out << boost::format("  \'ion-orbital-proj-%1%.data\' using (column(1)-(%2%)):(column(2)*(%3%)) w l title \'%4%\' linewidth 3,\\\n")
                % item.first
                % fermi_energy
                % (boost::contains(item.first, "spin-2") ? -1 : 1)
                % item.first;
        } else {
            out << boost::format("  \'ion-orbital-proj-%1%.data\' using (column(1)-(%2%)):(column(2)*(%3%)) w l title \'%4%\' linewidth 3\n")
                % item.first 
                % fermi_energy
                % (boost::contains(item.first, "spin-2") ? -1 : 1)
                % item.first;
        }
        i++;
    }

    out.close();

    out.open((fs::path(directory) / run_params["post-dir"] / "analysis.sh").string());
    out << "#!/bin/bash\n\n"
        << "#\n"
        << boost::format("cd %1%\n") % (fs::absolute(fs::path(directory)) / run_params["post-dir"]).string()
        << "\n"
        << "gnuplot dos.gnuplot\n";
    out.close();

    std::string cmd = "";
    cmd += "bash ";
    cmd += (fs::path(directory) / run_params["post-dir"] / "analysis.sh").string();
    std::system(cmd.c_str());
}

} // namespace atomsciflow::vasp::post
