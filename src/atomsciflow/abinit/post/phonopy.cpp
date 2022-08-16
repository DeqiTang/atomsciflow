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

#include "atomsciflow/abinit/post/phonopy.h"

#include <iomanip>
#include <algorithm>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <yaml-cpp/yaml.h>

namespace atomsciflow::abinit::post {

namespace ba = boost::algorithm;
namespace fs = boost::filesystem;

Phonopy::Phonopy() {
    this->set_run("program-out", "abinit.abo");
    this->set_run("output-json", "post-phonopy.json");
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
    stream << "BAND_POINTS = 51\n"; // 101
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
        << "cp ../phonopy_disp.yaml ./\n";
        
    stream << "# generate the FORCE_SETS\n";
    int num_gen_supercells = *std::max_element(supercells_index_int.begin(), supercells_index_int.end());
    if (num_gen_supercells <= 999) {
        stream << boost::format("phonopy -f ../run-supercell-{001..%1%}/abinit.abo\n") 
            % boost::io::group(
                std::setw(3), 
                std::setfill('0'),
                num_gen_supercells
            )
            ;
    } else if (num_gen_supercells <= 9999) {
        stream << boost::format("phonopy -f ../run-supercell-{001..999}/abinit.abo ../run-supercell-{1000..%1%}/abinit.abo\n") 
            % boost::io::group(
                std::setw(4), 
                std::setfill('0'),
                num_gen_supercells
            )
            ;
    }
    
    stream << "# plot the phonon dos\n"
        << "phonopy --abinit -p ./mesh.conf -c ../abinit.abi -s\n"
        << "# calclate thermal properties with specified sampling mesh\n"
        << "phonopy --abinit -t ./mesh.conf -c ../abinit.abi -s\n"
        << "# calculate projected phonon dos\n"
        << "phonopy --abinit -p ./pdos.conf -c ../abinit.abi -s\n"
        << "# calculate phonon band\n"
        << "phonopy --abinit -p ./band.conf -c ../abinit.abi -s\n"
        // << "phonopy-bandplot --gnuplot band.yaml"
        ;
    stream.close();

    std::string cmd = "";
    cmd += "bash ";
    cmd += (fs::path(directory) / run_params["post-dir"] / "analysis.sh").string();
    std::system(cmd.c_str());
}

} // namespace atomsciflow::abinit::post
