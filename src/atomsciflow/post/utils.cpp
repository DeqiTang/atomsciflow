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

#include "atomsciflow/post/utils.h"

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <numeric>
#include <yaml-cpp/yaml.h>

namespace atomsciflow::post {

namespace fs = boost::filesystem;

void extract_data_from_band_yaml(const std::string& directory) {

    std::vector<std::string> str_vec;
    std::vector<std::string> str_vec_1;
    std::vector<std::string> str_vec_2;
    YAML::Node band_yaml = YAML::LoadFile((fs::path(directory) / "band.yaml").string());
    auto npath = band_yaml["npath"].as<int>();
    auto segment_nqpoint = band_yaml["segment_nqpoint"];
    auto labels = band_yaml["labels"];
    std::ofstream band_data;
    band_data.open((fs::path(directory) / "band.data").string());
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
    band_gnuplot.open((fs::path(directory) / "band.gnuplot").string());
    band_gnuplot.setf(std::ios::fixed);
    band_gnuplot << "set terminal pngcairo enhanced\n";
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
    
    std::string cmd = "cd " + (fs::path(directory)).string() + "; gnuplot band.gnuplot";
    std::system(cmd.c_str());  
}

} // namespace atomsciflow::post
