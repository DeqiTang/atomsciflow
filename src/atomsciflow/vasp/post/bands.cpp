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

/// @file src/atomsciflow/vasp/post/bands.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Wed 09 Mar 2022 09:59:02 AM CST

#include "atomsciflow/vasp/post/bands.h"

#include <armadillo>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

namespace atomsciflow::vasp::post {

namespace fs = boost::filesystem;

Bands::Bands() {
    this->set_run("program-out", "OUTCAR");
    this->set_run("output-json", "post-band.json");
}

void Bands::_extract_data_from_vasprun_xml() {

    std::vector<std::string> str_vec;

    // extract the eigenvalues from vasprun.xml
    for (auto& spin : vasprun.get_child("modeling.calculation.eigenvalues.array.set")) {
        if (spin.first == "<xmlattr>") {
            continue;
        }
        std::string spin_label = boost::replace_all_copy(spin.second.get<std::string>("<xmlattr>.comment"), " ", "-");
        std::vector<std::map<std::string, std::vector<double>>> eigen_of_spin;
        for (auto& kpoint : spin.second) {
            if (kpoint.first == "<xmlattr>") {
                continue;
            }
            std::map<std::string, std::vector<double>> eigen;
            std::vector<double> energy;
            std::vector<double> occupation;
            for (auto& item : kpoint.second) {
                if (item.first == "<xmlattr>") {
                    continue;
                }
                boost::split(str_vec, item.second.data(), boost::is_any_of(" "), boost::token_compress_on);
                energy.push_back(boost::lexical_cast<double>(str_vec[1]));
                occupation.push_back(boost::lexical_cast<double>(str_vec[2]));
            }
            eigen["energy"] = energy;
            eigen["occupation"] = occupation;
            eigen_of_spin.push_back(eigen);
            this->eigenvalues[spin_label] = eigen_of_spin;
        }
    }

    // calculate the kcoords_1d
    std::vector<std::vector<double>> cell;

    for (auto& item1: vasprun.get_child("modeling")) {
        if (item1.first == "<xmlattr>") {
            continue;
        }
        if (item1.first == "structure" && item1.second.get<std::string>("<xmlattr>.name") == "finalpos") {
            for (auto& item2 : item1.second.get_child("crystal")) {
                if (item2.first == "<xmlattr>") {
                    continue;
                }
                if (item2.first == "varray" && item2.second.get<std::string>("<xmlattr>.name") == "basis") {
                    for (auto& item3 : item2.second) {
                        if (item3.first == "<xmlattr>") {
                            continue;
                        }
                        std::vector<double> vec;
                        boost::split(str_vec, item3.second.data(), boost::is_any_of(" "), boost::token_compress_on);
                        vec.push_back(boost::lexical_cast<double>(str_vec[1]));
                        vec.push_back(boost::lexical_cast<double>(str_vec[2]));
                        vec.push_back(boost::lexical_cast<double>(str_vec[3]));
                        cell.push_back(vec);
                    }
                    break;
                }
            }
            break;
        }
    }

    arma::mat latcell;
    latcell.set_size(3, 3);
    latcell.row(0) = arma::conv_to<arma::rowvec>::from(cell[0]);
    latcell.row(1) = arma::conv_to<arma::rowvec>::from(cell[1]);
    latcell.row(2) = arma::conv_to<arma::rowvec>::from(cell[2]);

    double volume = arma::dot(latcell.row(0), arma::cross(latcell.row(1), latcell.row(2)));

    arma::mat cell_k;
    cell_k.set_size(3, 3);
    cell_k.row(0) = arma::cross(latcell.row(1), latcell.row(2)) * 2 * arma::datum::pi / volume;
    cell_k.row(1) = arma::cross(latcell.row(2), latcell.row(0)) * 2 * arma::datum::pi / volume;
    cell_k.row(2) = arma::cross(latcell.row(0), latcell.row(1)) * 2 * arma::datum::pi / volume;

    this->kcoords_1d.push_back(0.0000000);
    for (int i = 1; i < this->kpath.coords.size(); i++) {
        if (this->kpath.links[i-1] != 0) {
            arma::rowvec point2 = this->kpath.coords[i][0] * cell_k.row(0) + this->kpath.coords[i][1] * cell_k.row(1) + this->kpath.coords[i][2] * cell_k.row(2);
            arma::rowvec point1 = this->kpath.coords[i-1][0] * cell_k.row(0) + this->kpath.coords[i-1][1] * cell_k.row(1) + this->kpath.coords[i-1][2] * cell_k.row(2);
            double distance_in_k_space = arma::norm(point2 - point1);
            double step = distance_in_k_space / (this->kpath.links[i-1] - 1);
            for (int j = 0; j < this->kpath.links[i-1]-1; j++) {
                this->kcoords_1d.push_back(this->kcoords_1d[this->kcoords_1d.size()-1] + step);
            }
            if (i == this->kpath.coords.size() - 1) {
                continue;
            } else {
                this->kcoords_1d.push_back(this->kcoords_1d[this->kcoords_1d.size()-1]);
            }
        }
    }    

    // get the fermi energy from vasprun.xml
    this->fermi_energy = this->vasprun.get<double>("modeling.calculation.dos.i");
}

void Bands::run(const std::string& directory) {
    Post::run(directory);

    pt::read_xml((fs::path(directory) / "vasprun.xml").string(), this->vasprun);
    this->_extract_data_from_vasprun_xml();

    std::vector<double> xtics_locs;
    std::vector<std::string> xticx_labels;
    xtics_locs.emplace_back(this->kcoords_1d[0]);
    for (int i = 1; i < this->kcoords_1d.size() - 1; i++) {
        if ((this->kcoords_1d[i] - kcoords_1d[i-1]) < 1.0e-5) {
            xtics_locs.emplace_back(kcoords_1d[i]);
        }
    }
    xtics_locs.emplace_back(kcoords_1d[kcoords_1d.size()-1]);

    xticx_labels.emplace_back(
        ba::to_upper_copy(this->kpath.labels[0]) != "GAMMA" ? ba::to_upper_copy(this->kpath.labels[0]) : "{/symbol G}"
    );
    for (int i = 1; i < this->kpath.coords.size(); i++) {
        if (this->kpath.links[i-1] != 0) {
            xticx_labels.emplace_back(
                ba::to_upper_copy(this->kpath.labels[i]) != "GAMMA" ? this->kpath.labels[i] : "{/symbol G}"
            );
        } else {
            xticx_labels[xticx_labels.size()-1] = xticx_labels[xticx_labels.size()-1] + " | " + ba::to_upper_copy(this->kpath.labels[i]);
        }
    }

    std::ofstream out;
    for (auto& spin : this->eigenvalues) {
        out.open((fs::path(directory) / (boost::format("post.dir/band-%1%.data") % spin.first).str()).string());
        out << boost::format("#kpoint eigenvalue-of-nbands:->%1% and the fermi energy is %2%\n") 
            % spin.second[0]["energy"].size()
            % this->fermi_energy;
        for (int i = 0; i < spin.second.size(); i++) {
            out << this->kcoords_1d[i];
            for (int j = 0; j < spin.second[i]["energy"].size(); j++) {
                out << " " << spin.second[i]["energy"][j];
            }
            out << "\n";
        }
        out.close();
    }

    out.open((fs::path(directory) / "post.dir/band.gnuplot").string());
    out << "set terminal png\n";
    out << "unset key\n";
    out << "set parametric\n";
    out << "set title 'Band structure' font ',15'\n";
    out << "set ylabel 'Energy (eV)' font ',15'\n";
    out << "set ytics font ',15'\n";
    out << "set xtics font ',15'\n";
    out << "set border linewidth 3\n";
    out << "set autoscale\n";
    // set a linestyle 1 to be the style for band lines
    out << "set style line 1 linecolor rgb \'black\' linetype 1 pointtype 1 linewidth 3\n";
    // set a linestyle 2 to be the style for the vertical high-symmetry kpoint line and horizontal fermi level line
    out << "set style line 2 linecolor rgb \'black\' linetype 1 pointtype 2 linewidth 0.5\n";
    
    out << "set xtics(";
    for (int i = 0; i < xticx_labels.size()-1; i++) {
        out << boost::format("\'%1%\' %2%, ") % xticx_labels[i] % xtics_locs[i];
    }
    out << boost::format("\'%1%\' %2%)\n") % xticx_labels[xticx_labels.size()-1] % xtics_locs[xtics_locs.size()-1];
    
    for (auto& x : xtics_locs) {
        out << boost::format("set arrow from %1%, graph 0 to %1%, graph 1 nohead linestyle 2\n") % x;
    }
    out << boost::format("set arrow from 0, 0 to %1%, 0 nohead linestyle 2\n") % xtics_locs[xtics_locs.size()-1];
    
    for (auto& spin : this->eigenvalues) {
        out << boost::format("set output 'band-%1%.png'\n") % spin.first;
        // the * in "for [j=2:*]" together with column(j) might not work for low version of gunuplot
        // out << boost::format("plot for [j=2:*] \'band-%1%.data\' using 1:(column(j) - %2%) w l notitle linestyle 1\n") 
            // % spin.first % this->fermi_energy;
        out << boost::format("plot for [j=2:%1%] \'band-%2%.data\' using 1:(column(j) - %3%) w l notitle linestyle 1\n") 
            % int(spin.second[0]["energy"].size() + 1)
            % spin.first % this->fermi_energy;
    }
    out.close();

    out.open((fs::path(directory) / run_params["post-dir"] / "analysis.sh").string());
    out << "#!/bin/bash\n\n"
        << "#\n"
        << boost::format("cd %1%\n") % (fs::absolute(fs::path(directory)) / run_params["post-dir"]).string()
        << "\n"
        << "gnuplot band.gnuplot\n";
    out.close();

    std::string cmd = "";
    cmd += "bash ";
    cmd += (fs::path(directory) / run_params["post-dir"] / "analysis.sh").string();
    std::system(cmd.c_str());
}

} // namespace atomsciflow::vasp::post
