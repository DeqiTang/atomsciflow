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

#include "atomsciflow/siesta/post/dos.h"

#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <armadillo>

namespace atomsciflow::siesta::post {

namespace fs = boost::filesystem;

Dos::Dos() {
    this->set_run("program-out", "siesta.out");
    this->set_run("output-json", "post-dos.json");
}

void Dos::run(const std::string& directory) {
    Post::run(directory);
    
    std::ofstream out;

    pt::ptree pdosxml;
    pt::read_xml((fs::path(directory) / "siesta-run.PDOS.xml").string(), pdosxml);

    int num_spins = pdosxml.get<int>("pdos.nspin");
    double fermi_energy = pdosxml.get<double>("pdos.fermi_energy");
    arma::rowvec energies = arma::rowvec(pdosxml.get<std::string>("pdos.energy_values"));

    std::map<std::string, arma::rowvec> elem_proj;
    for (auto& item : pdosxml.get_child("pdos")) {
        if (item.first != "orbital") {
            continue;
        }
        std::string element = item.second.get<std::string>("<xmlattr>.species");
        for (int spin = 0; spin < num_spins; spin++) {
            auto key = (boost::format("%1%-spin-%2%")%element%(spin+1)).str();
            if (elem_proj.find(key) == elem_proj.end()) {
                // remember to initialize the rowvec with 0
                elem_proj[key] = arma::conv_to<arma::rowvec>::from(arma::zeros(energies.n_cols));
            }
            elem_proj[key] += arma::rowvec(item.second.get<std::string>("data"))(arma::span(spin*energies.size(), (spin+1)*energies.size()-1));
        }
    }


    for (auto& item : elem_proj) {
        out.open((fs::path(directory) / "post.dir" / (boost::format("elem-proj-%1%.data")%item.first).str()));
        for (int i = 0; i < energies.size(); i++) {
            out << boost::format("%1$10.8f %2$10.8f\n") % energies.at(i) % item.second.at(i);
        }
        out.close();
    }

    std::map<int, std::string> map_l;
    map_l[0] = "s";
    map_l[1] = "p";
    map_l[2] = "d";
    map_l[3] = "f";

    std::map<std::string, arma::rowvec> elem_l_proj;
    for (auto& item : pdosxml.get_child("pdos")) {
        if (item.first != "orbital") {
            continue;
        }
        std::string element = item.second.get<std::string>("<xmlattr>.species");
        std::string n = item.second.get<std::string>("<xmlattr>.n");
        std::string l = map_l[item.second.get<int>("<xmlattr>.l")];
        for (int spin = 0; spin < num_spins; spin++) {
            auto key = (boost::format("%1%-%2%%3%-spin-%4%")%element%n%l%(spin+1)).str();
            if (elem_l_proj.find(key) == elem_l_proj.end()) {
                // remember to initialize the rowvec with 0
                elem_l_proj[key] = arma::conv_to<arma::rowvec>::from(arma::zeros(energies.n_cols));
            }
            elem_l_proj[key] += arma::rowvec(item.second.get<std::string>("data"))(arma::span(spin*energies.size(), (spin+1)*energies.size()-1));
        }
    }

    for (auto& item : elem_l_proj) {
        out.open((fs::path(directory) / "post.dir" / (boost::format("elem-l-proj-%1%.data")%item.first).str()));
        for (int i = 0; i < energies.size(); i++) {
            out << boost::format("%1$10.8f %2$10.8f\n") % energies.at(i) % item.second.at(i);
        }
        out.close();
    }    

    std::map<std::string, arma::rowvec> elem_l_m_proj;
    for (auto& item : pdosxml.get_child("pdos")) {
        if (item.first != "orbital") {
            continue;
        }
        std::string element = item.second.get<std::string>("<xmlattr>.species");
        std::string n = item.second.get<std::string>("<xmlattr>.n");
        std::string l = map_l[item.second.get<int>("<xmlattr>.l")];
        std::string m = item.second.get<std::string>("<xmlattr>.m");
        for (int spin = 0; spin < num_spins; spin++) {
            auto key = (boost::format("%1%-%2%%3%(%4%)-spin-%5%")%element%n%l%m%(spin+1)).str();
            if (elem_l_m_proj.find(key) == elem_l_m_proj.end()) {
                // remember to initialize the rowvec with 0
                elem_l_m_proj[key] = arma::conv_to<arma::rowvec>::from(arma::zeros(energies.n_cols));
            }
            elem_l_m_proj[key] += arma::rowvec(item.second.get<std::string>("data"))(arma::span(spin*energies.size(), (spin+1)*energies.size()-1));
        }
    }

    for (auto& item : elem_l_m_proj) {
        out.open((fs::path(directory) / "post.dir" / (boost::format("elem-l-m-proj-%1%.data")%item.first).str()));
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

    out << "set output \'elem-proj.png\'\n";
    out << "plot \\\n";        
    i = 0;
    for (auto& item : elem_proj) {
        if (i != elem_proj.size() - 1) {
            out << boost::format("  \'elem-proj-%1%.data\' using (column(1)-(%2%)):(column(2)*(%3%)) w l title \'%4%\' linewidth 3,\\\n")
                % item.first
                % fermi_energy
                % (boost::contains(item.first, "spin-2") ? -1 : 1)
                % item.first;
        } else {
            out << boost::format("  \'elem-proj-%1%.data\' using (column(1)-(%2%)):(column(2)*(%3%)) w l title \'%4%\' linewidth 3\n")
                % item.first 
                % fermi_energy
                % (boost::contains(item.first, "spin-2") ? -1 : 1)
                % item.first;
        }
        i++;
    }

    out << "set output \'elem-l-proj.png\'\n";
    out << "plot \\\n";        
    i = 0;
    for (auto& item : elem_l_proj) {
        if (i != elem_l_proj.size() - 1) {
            out << boost::format("  \'elem-l-proj-%1%.data\' using (column(1)-(%2%)):(column(2)*(%3%)) w l title \'%4%\' linewidth 3,\\\n")
                % item.first
                % fermi_energy
                % (boost::contains(item.first, "spin-2") ? -1 : 1)
                % item.first;
        } else {
            out << boost::format("  \'elem-l-proj-%1%.data\' using (column(1)-(%2%)):(column(2)*(%3%)) w l title \'%4%\' linewidth 3\n")
                % item.first 
                % fermi_energy
                % (boost::contains(item.first, "spin-2") ? -1 : 1)
                % item.first;
        }
        i++;
    }

    out << "set output \'elem-l-m-proj.png\'\n";
    out << "plot \\\n";        
    i = 0;
    for (auto& item : elem_l_m_proj) {
        if (i != elem_l_m_proj.size() - 1) {
            out << boost::format("  \'elem-l-m-proj-%1%.data\' using (column(1)-(%2%)):(column(2)*(%3%)) w l title \'%4%\' linewidth 3,\\\n")
                % item.first
                % fermi_energy
                % (boost::contains(item.first, "spin-2") ? -1 : 1)
                % item.first;
        } else {
            out << boost::format("  \'elem-l-m-proj-%1%.data\' using (column(1)-(%2%)):(column(2)*(%3%)) w l title \'%4%\' linewidth 3\n")
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

} // namespace atomsciflow::siesta::post
