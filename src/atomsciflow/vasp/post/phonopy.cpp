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

/// @file src/atomsciflow/vasp/post/phonopy.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Mon 28 Feb 2022 11:06:10 PM CST

#include "atomsciflow/vasp/post/phonopy.h"

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <numeric>

namespace atomsciflow {

namespace fs = boost::filesystem;
namespace ba = boost::algorithm;

PhonopyPost::PhonopyPost() {
    supercell_n = {1, 1, 1};
    mp = {8, 8, 8};
}

PhonopyPost::~PhonopyPost() {

}

void PhonopyPost::get_kpath(const Kpath& kpath) {
    this->kpath = kpath;
}

void PhonopyPost::get_xyz(Xyz& xyz) {
    this->xyz = xyz;
}

void PhonopyPost::get_xyz(std::string& xyzfile) {
    this->xyz.read_xyz_file(xyzfile);
}

void PhonopyPost::process(const std::string& directory, int tmax = 1500) {
 
    auto dir_path = fs::absolute(directory);  
    std::string cmd = "";
    cmd += "ls " + dir_path.string() + " | grep 'POSCAR-' > " + (dir_path / "pos.data").string();
    std::system(cmd.c_str());

    std::vector<std::string> disps;

    std::ifstream pos_data_file;
    pos_data_file.open((dir_path / "pos.data").string());
    std::string line;
    std::vector<std::string> str_vec;
    std::vector<std::string> str_vec_1;
    std::vector<std::string> str_vec_2;
    while (std::getline(pos_data_file, line)) {
        boost::split(str_vec_1, line, boost::is_any_of("\n"), boost::token_compress_on);
        boost::split(str_vec_2, str_vec_1[0], boost::is_any_of("-"));
        disps.push_back(str_vec_2[1]);
    }
    pos_data_file.close();

    fs::create_directory(dir_path / "post-processing");
    
    std::ofstream mesh_conf_file;
    mesh_conf_file.open((dir_path / "post-processing/mesh.conf").string());
    mesh_conf_file.setf(std::ios::fixed);
    mesh_conf_file << "ATOM_NAME =";
    for (const auto& element : this->xyz.elements_set) {
        mesh_conf_file << " " << element;
    }
    mesh_conf_file << "\n";
    mesh_conf_file << "DIM = " << this->supercell_n[0] << " " << this->supercell_n[1] << " " << this->supercell_n[2] << "\n";
    mesh_conf_file << "MP = " << this->mp[0] << " " << this->mp[1] << " " << this->mp[2] << "\n";
    mesh_conf_file.close();
  
    std::ofstream pdos_conf_file;
    pdos_conf_file.open((dir_path / "post-processing/pdos.conf").string());
    pdos_conf_file.setf(std::ios::fixed);
    pdos_conf_file << "ATOM_NAME =";
    for (const auto& element : this->xyz.elements_set) {
        pdos_conf_file << " " << element;
    }
    pdos_conf_file << "\n";
    pdos_conf_file << "DIM = " << this->supercell_n[0] << " " << this->supercell_n[1] << " " << this->supercell_n[2] << "\n";
    pdos_conf_file << "MP = " << this->mp[0] << " " << this->mp[1] << " " << this->mp[2] << "\n";
    pdos_conf_file << "PDOS = 1 2, 3 4 5 5\n";
    pdos_conf_file.close();

    std::ofstream band_conf_file;
    band_conf_file.open((dir_path / "post-processing/band.conf").string());
    band_conf_file.setf(std::ios::fixed);
    band_conf_file << "ATOM_NAME =";
    for (const auto& element : this->xyz.elements_set) {
        band_conf_file << " " << element;
    }
    band_conf_file << "\n";
    band_conf_file << "PRIMITIVE_AXES = AUTO\n";
    band_conf_file << "GAMMA_CENTER = .TRUE.\n";
    band_conf_file << "BAND_POINTS = 101\n";
    band_conf_file << "BAND_CONNECTION = .TRUE.\n";
    band_conf_file << "DIM = " << this->supercell_n[0] << " " << this->supercell_n[1] << " " << this->supercell_n[2] << "\n";
    band_conf_file << "MP = " << this->mp[0] << " " << this->mp[1] << " " << this->mp[2] << "\n";
    band_conf_file << "BAND = ";
    for (int i = 0; i < this->kpath.coords.size(); i++) {
        if (this->kpath.links[i] != 0) {
            band_conf_file << " " << this->kpath.coords[i][0] << " " << this->kpath.coords[i][1] << " " << this->kpath.coords[i][2];
        } else {
            band_conf_file << " " << this->kpath.coords[i][0] << " " << this->kpath.coords[i][1] << " " << this->kpath.coords[i][2] << ",";
        }
    } 
    band_conf_file << "\n";
    band_conf_file << "BAND_LABELS =";
    for (int i = 0; i < this->kpath.coords.size() - 1; i++) {
        if (this->kpath.links[i] != 0) {
            if (ba::to_upper_copy(this->kpath.labels[i]) == "GAMMA") {
                band_conf_file << " $\\Gamma$";
            } else {
                band_conf_file << " $" << this->kpath.labels[i] << "$";
            } 
        } else {
            if (ba::to_upper_copy(this->kpath.labels[i]) == "GAMMA") {
                band_conf_file << " $\\Gamma,$";
            } else {
                band_conf_file << " $" << this->kpath.labels[i] << "$,";
            }
        }
    }
    if (ba::to_upper_copy(this->kpath.labels[-1]) == "GAMMA") {
        band_conf_file << " $\\Gamma$";
    } else {
        band_conf_file << " $" << this->kpath.labels[-1] << "$";
    }
    band_conf_file << "\n";
    band_conf_file.close();
    
    std::ofstream phonopy_analysis_sh;
    phonopy_analysis_sh.open((dir_path / "post-processing/phonopy-analysis.h").string());
    phonopy_analysis_sh << "#!/bin/bash\n\n";
    phonopy_analysis_sh << "cp ../phonopy_disp.yaml ./\n";
    phonopy_analysis_sh << "cp ../disp.yaml ./\n";
    phonopy_analysis_sh << "cp ../POSCAR ./\n";
    phonopy_analysis_sh << "# generate the FORCE_SET\n";
    phonopy_analysis_sh << "phonopy -f ../disp-{001.." << disps[-1] << "}/vasprun.xml\n";
    phonopy_analysis_sh << "# plot The density of states (DOS)\n";
    phonopy_analysis_sh << "phonopy -p mesh.conf -s\n";
    phonopy_analysis_sh << "# Thermal properties are calculated with the sampling mesh by:\n";
    phonopy_analysis_sh << "phonopy -t mesh.conf --tmax " << tmax << "\n";
    phonopy_analysis_sh << "# Thermal properties can be plotted by:\n";
    phonopy_analysis_sh << "phonopy -t -p mesh.conf -s --tmax " << tmax << "\n";
    phonopy_analysis_sh << "# calculate Projected DOS and plot it\n";
    phonopy_analysis_sh << "phonopy -p pdos.conf -s\n";
    phonopy_analysis_sh << "# plot band structure\n";
    phonopy_analysis_sh << "phonopy -p band.conf -s\n";
    phonopy_analysis_sh.close();
  
    //std::system("cd post-processing; bash phonopy-analysis.sh; cd ../");
    cmd = "cd " + (dir_path / "post-processing").string() + "; " + "bash phonopy-analysis.sh;";
    std::system(cmd.c_str()); 

    YAML::Node yaml_node = YAML::Load("[1, 2, 3, 4, 5, 6]");
    int size = yaml_node.size();
    //for (int i = 0; i < size; i++) {
    //    std::cout << yaml_node[i].as<int>() << std::endl;
    //}
    YAML::Node band_yaml = YAML::LoadFile((dir_path / "post-processing/band.yaml").string());
    auto npath = band_yaml["npath"].as<int>();
    auto segment_nqpoint = band_yaml["segment_nqpoint"];
    auto labels = band_yaml["labels"];
    std::ofstream band_data;
    band_data.open((dir_path / "post-processing/band.data").string());
    band_data.setf(std::ios::fixed);
    band_data << "#kpoint(segmented) band(1-n)\n";
    int nqpoint = band_yaml["nqpoint"].as<int>();
    int nband = band_yaml["phonon"][0]["band"].size();

    int start = 0;
    int end = 0;
    for (int s = 0; s < segment_nqpoint.size(); s++) {
        if (s == 0) {
            start = 0;
            end = segment_nqpoint[0].as<int>() - 1;
        } else {
            for (int i = 0; i < s; i ++) {
                start += segment_nqpoint[i].as<int>();
            }
            end = start + segment_nqpoint[s].as<int>() - 1;
        }
        for (int i = start; i < end + 1; i++) {
            band_data << band_yaml["phonon"][i]["distance"].as<double>();
            for (const auto& band : band_yaml["phonon"][i]["band"]) {
                band_data << " " << band["frequency"].as<double>();
            }
            band_data << "\n";
        }
        band_data << "\n";
    }
    band_data.close();

    std::vector<double> locs;
    std::vector<std::string> labels_for_gnuplot;
    if (labels[0][0].as<std::string>() != "$\\Gamma$") {
        boost::split(str_vec_1, labels[0][0].as<std::string>(), boost::is_any_of("$"));
        labels_for_gnuplot.push_back(str_vec_1[1]);
    } else {
        labels_for_gnuplot.push_back("{/symbol G}");
    }
    if (labels[0][1].as<std::string>() != "$\\Gamma$") {
        boost::split(str_vec_1, labels[0][1].as<std::string>(), boost::is_any_of("$"));
        labels_for_gnuplot.push_back(str_vec_1[1]);
    } else {
        labels_for_gnuplot.push_back("{/symbol G}");
    }   
    int iqpoint = 0;
    locs.push_back(band_yaml["phonon"][iqpoint]["distance"].as<double>());
    iqpoint += segment_nqpoint[0].as<int>() - 1;
    locs.push_back(band_yaml["phonon"][iqpoint]["distance"].as<double>());
    for (int ipath = 1; ipath < npath; ipath++) {
        if (labels[ipath][0].as<std::string>() == labels[ipath-1][1].as<std::string>()) {
            if (labels[ipath][1].as<std::string>() == "$\\Gamma$") {
                labels_for_gnuplot.push_back("{/symbol G}");
            } else {
                boost::split(str_vec, labels[ipath][1].as<std::string>(), boost::is_any_of("$"));
                labels_for_gnuplot.push_back(str_vec[1]);
            }
        } else {
            if (labels[ipath-1][1].as<std::string>() == "$\\Gamma$") {
                boost::split(str_vec, labels[ipath][0].as<std::string>(), boost::is_any_of("$"));
                labels_for_gnuplot.push_back(std::string("{/symbol G}") + "|" + str_vec[1]);
            } else if (labels[ipath][0].as<std::string>() == "$\\Gamma$") {
                boost::split(str_vec, labels[ipath-1][1].as<std::string>(), boost::is_any_of("|"));
                labels_for_gnuplot.push_back(str_vec[1] + "|" + "{/symbol G}");
            } else {
                boost::split(str_vec, labels[ipath-1][1].as<std::string>(), boost::is_any_of("|"));
                boost::split(str_vec_1, labels[ipath][0].as<std::string>(), boost::is_any_of("$"));
                labels_for_gnuplot.push_back(str_vec[1] + "|" + str_vec_1[1]);
            }   
            if (labels[ipath][1].as<std::string>() == "$\\Gamma$") {
                labels_for_gnuplot.push_back("{/symbol G}");
            } else {
                boost::split(str_vec, labels[ipath][1].as<std::string>(), boost::is_any_of("$"));
                labels_for_gnuplot.push_back(str_vec[1]);
            }
        }
        iqpoint += segment_nqpoint[ipath].as<int>();
        locs.push_back(band_yaml["phonon"][iqpoint]["distance"].as<double>());
    }

    std::ofstream band_gnuplot;
    band_gnuplot.open((dir_path / "post-processing/band.gnuplot").string());
    band_gnuplot.setf(std::ios::fixed);
    band_gnuplot << "set terminal gif\n";
    band_gnuplot << "set output 'phonon_band.gif'\n";
    band_gnuplot << "unset key\n";
    band_gnuplot << "set parametric\n";
    band_gnuplot << "set xlabel 'K'\n";
    band_gnuplot << "set ylabel 'Frequency(THz)'\n";
    band_gnuplot << "set xticks(";
    for (int j = 0; j < labels_for_gnuplot.size() - 1; j++) {
        band_gnuplot << "'" << labels_for_gnuplot[j] << "' " << locs[j] << "\n";
    }
    band_gnuplot << "'" << labels_for_gnuplot[-1] << "' " << locs[-1] << "\n";
    band_gnuplot << "set grid xtics ytics\n";
    band_gnuplot << "set autoscale\n";
    band_gnuplot << "plot for [i=2:" << (nband + 1) << ":1] 'band.data' using 1:i w 1\n";
    band_gnuplot.close();
    cmd = "cd " + (dir_path / "post-processing").string() + "; gnuplot band.gnuplot";
    std::system(cmd.c_str());
}

void PhonopyPost::run(const std::string& directory) {
    this->process(directory, 1200);
}

} // namespace atomsciflow
