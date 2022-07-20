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
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <numeric>

namespace atomsciflow::vasp::post {

namespace fs = boost::filesystem;
namespace ba = boost::algorithm;
namespace pt = boost::property_tree;

Phonopy::Phonopy() {

}

Phonopy::~Phonopy() {

}

void Phonopy::run(const std::string& directory) {

    YAML::Node phonopy_disp_yaml = YAML::LoadFile((fs::path(directory) / "phonopy_disp.yaml").string());
    std::vector<std::string> vec_str;
    std::vector<int> dim;
    boost::split(vec_str, phonopy_disp_yaml["phonopy"]["configuration"]["dim"].as<std::string>(), boost::is_any_of(" "));
    for (auto& item : vec_str) {
        dim.push_back(boost::lexical_cast<int>(item));
    }
    std::vector<int> mesh{6, 6, 6};

    fs::create_directory(fs::path(directory) / run_params["post-dir"]);
    std::ofstream stream;
    
    stream.open((fs::path(directory) / run_params["post-dir"] / "mesh.conf").string());
    stream << boost::format("DIM = %1% %2% %3%\n") % dim[0] % dim[1] % dim[2];
    stream << boost::format("MP = %1% %2% %3%\n") % mesh[0] % mesh[1] % mesh[2];
    stream.close();

    stream.open((fs::path(directory) / run_params["post-dir"] / "pdos.conf").string());
    stream << boost::format("DIM = %1% %2% %3%\n") % dim[0] % dim[1] % dim[2];
    stream << boost::format("MP = %1% %2% %3%\n") % mesh[0] % mesh[1] % mesh[2];
    stream << "PDOS = 1 2, 3 4 5 5\n";
    stream.close();

    stream.open((fs::path(directory) / run_params["post-dir"] / "band.conf").string());
    stream << "PRIMIMTIVE_AXES = AUTO\n";
    stream << "GAMMA_CENTER = .TRUE.\n";
    stream << "BAND_POINTS = 11\n"; // 101
    stream << "BAND_CONNECTION = .TRUE.\n";
    stream << boost::format("DIM = %1% %2% %3%\n") % dim[0] % dim[1] % dim[2];
    stream << "BAND =";
    for (int i = 0; i < kpath.coords.size() - 1; i++) {
        if (kpath.links[i] != 0) {
            stream << boost::format(" %1% %2% %3%") % kpath.coords[i][0] % kpath.coords[i][1] % kpath.coords[i][2];
        } else {
            stream << boost::format(" %1% %2% %3%,") % kpath.coords[i][0] % kpath.coords[i][1] % kpath.coords[i][2];
        }
    }
    stream << boost::format(" %1% %2% %3%") % kpath.coords[kpath.coords.size()-1][0] % kpath.coords[kpath.coords.size()-1][1] % kpath.coords[kpath.coords.size()-1][2];

    stream << "\n";
    stream << "BAND_LABELS =";
    for (int i = 0; i < kpath.coords.size() - 1; i++) {
        if (kpath.links[i] != 0) {
            if (ba::to_upper_copy(kpath.labels[i]) == "GAMMA") {
                stream << " $\\Gamma$";
            } else {
                stream << boost::format(" $%1%$") % kpath.labels[i];
            }
        } else {
            if (ba::to_upper_copy(kpath.labels[i]) == "GAMMA") {
                stream << " $\\Gamma$,";
            } else {
                stream << boost::format(" $%1%$,") % kpath.labels[i];
            }            
        }
    }
    if (ba::to_upper_copy(kpath.labels[kpath.labels.size()-1]) == "GAMMA") {
        stream << " $\\Gamma$";
    } else {
        stream << boost::format(" $%1%$") % kpath.labels[kpath.labels.size()-1];
    }   
    stream << "\n";
    stream.close();

    std::vector<std::string> supercells_index;
    std::vector<std::string> filenames;
    for (auto& filename : fs::directory_iterator(directory)) {
        filenames.push_back(filename.path().filename().string());
    }
    std::regex pat1("[a-zA-Z\\_*\\-*0-9]*-supercell-\\d+$");
    std::regex pat2("-supercell-\\d+");
    std::regex pat3("\\d+");
    std::smatch m1;
    std::smatch m2;
    std::smatch m3;
    for (auto& item : filenames) {
        if (std::regex_search(item, m1, pat1)) {
            std::regex_search(item, m2, pat2);
            std::string tmp_str = m2.str(0);
            std::regex_search(tmp_str, m3, pat3);
            supercells_index.push_back(m3.str(0));
        }
    }
    std::vector<int> supercells_index_int;
    for (auto& item : supercells_index) {
        supercells_index_int.push_back(boost::lexical_cast<int>(item));
    }

    stream.open((fs::path(directory) / run_params["post-dir"] / "analysis.sh").string());
    stream << "#!/bin/bash\n\n"
        << "#\n"
        << boost::format("cd %1%\n") % (fs::absolute(fs::path(directory)) / run_params["post-dir"]).string()
        << "\n"
        << "cp ../phonopy_disp.yaml ./\n"
        << "cp ../POSCAR ./\n";

    stream << "# generate the FORCE_SETS\n";
    int num_gen_supercells = *std::max_element(supercells_index_int.begin(), supercells_index_int.end());
    if (num_gen_supercells <= 999) {
        stream << boost::format("phonopy -f ../run-supercell-{001..%1%}/vasprun.xml\n") 
            % boost::io::group(
                std::setw(3), 
                std::setfill('0'),
                num_gen_supercells
            );
    } else if (num_gen_supercells <= 9999) {
        stream << boost::format("phonopy -f ../run-supercell-{001..999}/vasprun.xml ../run-supercell-{1000..%1%}/vasprun.xml\n") 
            % boost::io::group(
                std::setw(4), 
                std::setfill('0'),
                num_gen_supercells
            );
    }
    
    stream << "# plot the phonon dos\n"
        << "phonopy -p ./mesh.conf -c ./POSCAR -s\n"
        << "# calclate thermal properties with specified sampling mesh\n"
        << "phonopy -t ./mesh.conf -c ./POSCAR -s\n"
        << "# calculate projected phonon dos\n"
        << "phonopy -p ./pdos.conf -c ./POSCAR -s\n"
        << "# calculate phonon band\n"
        << "phonopy -p ./band.conf -c ./POSCAR -s\n"
        // << "phonopy-bandplot --gnuplot band.yaml"
        ;
    stream.close();

    std::string cmd = "";
    cmd += "bash ";
    cmd += (fs::path(directory) / run_params["post-dir"] / "analysis.sh").string();
    std::system(cmd.c_str());
}

void Phonopy::extract_data(const std::string& directory) {

    std::vector<std::string> str_vec;
    std::vector<std::string> str_vec_1;
    std::vector<std::string> str_vec_2;
    YAML::Node band_yaml = YAML::LoadFile((fs::path(directory) / run_params["post-dir"] / "band.yaml").string());
    auto npath = band_yaml["npath"].as<int>();
    auto segment_nqpoint = band_yaml["segment_nqpoint"];
    auto labels = band_yaml["labels"];
    std::ofstream band_data;
    band_data.open((fs::path(directory) / run_params["post-dir"] / "band.data").string());
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
            start = 0;
            for (int i = 0; i < s; i++) {
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
    if (!boost::contains(labels[0][0].as<std::string>(), "Gamma")) {
        boost::split(str_vec_1, labels[0][0].as<std::string>(), boost::is_any_of("$"));
        labels_for_gnuplot.push_back(str_vec_1[1]);
    } else {
        labels_for_gnuplot.push_back("{/symbol G}");
    }
    if (!boost::contains(labels[0][1].as<std::string>(), "Gamma")) {
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
            if (boost::contains(labels[ipath][1].as<std::string>(), "Gamma")) {
                labels_for_gnuplot.push_back("{/symbol G}");
            } else {
                boost::split(str_vec, labels[ipath][1].as<std::string>(), boost::is_any_of("$"));
                labels_for_gnuplot.push_back(str_vec[1]);
            }
        } else {
            if (boost::contains(labels[ipath-1][1].as<std::string>(), "Gamma")) {
                boost::split(str_vec, labels[ipath][0].as<std::string>(), boost::is_any_of("$"));
                labels_for_gnuplot[labels_for_gnuplot.size()-1] = std::string("{/symbol G}") + "|" + str_vec[1];
            } else if (boost::contains(labels[ipath][0].as<std::string>(), "Gamma")) {
                boost::split(str_vec, labels[ipath-1][1].as<std::string>(), boost::is_any_of("&"));
                labels_for_gnuplot[labels_for_gnuplot.size()-1] = str_vec[1] + "|" + "{/symbol G}";
            } else {
                boost::split(str_vec, labels[ipath-1][1].as<std::string>(), boost::is_any_of("$"));
                boost::split(str_vec_1, labels[ipath][0].as<std::string>(), boost::is_any_of("$"));
                labels_for_gnuplot[labels_for_gnuplot.size()-1] = str_vec[1] + "|" + str_vec_1[1];
            }

            if (boost::contains(labels[ipath][1].as<std::string>(), "Gamma")) {
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
    band_gnuplot.open((fs::path(directory) / run_params["post-dir"] / "band.gnuplot").string());
    band_gnuplot.setf(std::ios::fixed);
    band_gnuplot << "set terminal png\n";
    band_gnuplot << "set output 'phonon_band.png'\n";
    band_gnuplot << "unset key\n";
    band_gnuplot << "set parametric\n";
    band_gnuplot << "set xlabel 'K'\n";
    band_gnuplot << "set ylabel 'Frequency(THz)'\n";
    band_gnuplot << "set xtics(";
    for (int j = 0; j < labels_for_gnuplot.size() - 1; j++) {
        band_gnuplot << "'" << labels_for_gnuplot[j] << "' " << locs[j] <<", ";
    }
    band_gnuplot << "'" << labels_for_gnuplot[labels_for_gnuplot.size()-1] << "' " << locs[locs.size()-1];
    band_gnuplot << ")\n";
    band_gnuplot << "set grid xtics ytics\n";
    band_gnuplot << "set autoscale\n";
    band_gnuplot << "plot for [i=2:" << (nband + 1) << ":1] 'band.data' using 1:i w l\n";
    band_gnuplot.close();
    
    std::string cmd = "cd " + (fs::path(directory) / run_params["post-dir"]).string() + "; gnuplot band.gnuplot";
    std::system(cmd.c_str());    
}

} // namespace atomsciflow
