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

#include "atomsciflow/cp2k/post/bands.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "atomsciflow/base/xyz.h"

namespace atomsciflow::cp2k::post {

namespace ba = boost::algorithm;

Bands::Bands() {    
    this->ha_to_ev = 27.211324570273; 
    this->set_run("program-out", "cp2k.out");
    this->set_run("output-json", "post-band.json");    
}

Bands::~Bands() {

}

void Bands::run(const std::string& directory) {
    Post::run(directory);

    std::vector<std::string> str_vec_1;
    std::vector<std::string> str_vec_2;
    std::vector<std::string> str_vec_3;
    std::vector<std::string> str_vec_4;
    std::string line;

    std::ifstream fin;
    std::ofstream out;

    std::vector<std::string> cp2k_out_lines;
    fin.open((fs::path(directory) / "cp2k.out").string());
    while (std::getline(fin, line)) {
        cp2k_out_lines.push_back(line);
    }
    fin.close();

    // get fermi energy
    // need to set force_eval/dft/print/mo_cubes/nlumo to print the Fermi energy
    double fermi_in_ev = 0;
    for (int i = cp2k_out_lines.size()-1; i >=0; i--) { // get the final fermi energy
        line = cp2k_out_lines[i];
        if (boost::contains(line, "Fermi")) {
            std::regex pat("\\-*\\d+\\.\\d+");
            std::smatch m1;
            std::regex_search(line, m1, pat);
            fermi_in_ev = boost::lexical_cast<double>(m1.str(0));
            break;
        }
    }

    // get xyz
    Xyz xyz;
    std::regex pat_xyz("\\.xyz$");
    std::smatch match_xyz;
    for (auto& item : fs::directory_iterator(directory)) {
        std::string filename = item.path().filename().string();
        if (std::regex_search(filename, match_xyz, pat_xyz)) {
            xyz.read_xyz_file((fs::path(directory) / filename).string());
        }
    }

    arma::mat latcell;
    latcell.set_size(3, 3);
    latcell.row(0) = arma::conv_to<arma::rowvec>::from(xyz.cell[0]);
    latcell.row(1) = arma::conv_to<arma::rowvec>::from(xyz.cell[1]);
    latcell.row(2) = arma::conv_to<arma::rowvec>::from(xyz.cell[2]);

    double volume = arma::dot(latcell.row(0), arma::cross(latcell.row(1), latcell.row(2)));

    arma::mat cell_k;
    cell_k.set_size(3, 3);
    cell_k.row(0) = arma::cross(latcell.row(1), latcell.row(2)) * 2 * arma::datum::pi / volume;
    cell_k.row(1) = arma::cross(latcell.row(2), latcell.row(0)) * 2 * arma::datum::pi / volume;
    cell_k.row(2) = arma::cross(latcell.row(0), latcell.row(1)) * 2 * arma::datum::pi / volume;
    
    // get band data
    std::vector<std::string> bands_lines;
    fin.open((fs::path(directory) / "bands.bs").string());
    while (std::getline(fin, line)) {
        bands_lines.push_back(line);
    }
    fin.close();

    int num_spins = 1;
    for (auto& line : bands_lines) {
        if (boost::contains(line, "Spin 2:")) {
            num_spins = 2;
            break;
        }
    }
    int num_bands = 0;
    for (auto& line : bands_lines) {
        if (boost::contains(line, "# Set")) {
            boost::split(str_vec_1, line, boost::is_any_of(" "), boost::token_compress_on);
            num_bands = boost::lexical_cast<int>(str_vec_1[8]);
            break;
        }
    }

    for (int i = 0; i < bands_lines.size(); i++) {
        boost::split(str_vec_1, bands_lines[i], boost::is_any_of(" "), boost::token_compress_on);
        if (str_vec_1[1] == "Set") {
            this->kpoint_sets.push_back(std::make_shared<KpointSet>());
            auto& kpoint_set = kpoint_sets[kpoint_sets.size()-1];
            kpoint_set->num_kpoints = boost::lexical_cast<int>(str_vec_1[6]);
            kpoint_set->num_bands = boost::lexical_cast<int>(str_vec_1[8]);
            kpoint_set->special_coord_1.set_size(1, 3);
            boost::split(str_vec_2, bands_lines[i+1], boost::is_any_of(" "), boost::token_compress_on);
            kpoint_set->special_coord_1.at(0) = boost::lexical_cast<double>(str_vec_2[4]);
            kpoint_set->special_coord_1.at(1) = boost::lexical_cast<double>(str_vec_2[5]);
            kpoint_set->special_coord_1.at(2) = boost::lexical_cast<double>(str_vec_2[6]);
            kpoint_set->special_label_1 = str_vec_2[7];            
            kpoint_set->special_coord_2.set_size(1, 3);
            boost::split(str_vec_2, bands_lines[i+2], boost::is_any_of(" "), boost::token_compress_on);
            kpoint_set->special_coord_2.at(0) = boost::lexical_cast<double>(str_vec_2[4]);
            kpoint_set->special_coord_2.at(1) = boost::lexical_cast<double>(str_vec_2[5]);
            kpoint_set->special_coord_2.at(2) = boost::lexical_cast<double>(str_vec_2[6]);
            kpoint_set->special_label_2 = str_vec_2[7];
            for (int k = 0; k < (kpoint_set->num_kpoints*num_spins); k++) {
                kpoint_set->kpoint.push_back(std::make_shared<Kpoint>());
                auto& kpoint = kpoint_set->kpoint[kpoint_set->kpoint.size()-1];
                kpoint->coord.set_size(1, 3);
                boost::split(str_vec_3, bands_lines[i+3+k*(kpoint_set->num_bands+2)], boost::is_any_of(" "), boost::token_compress_on);
                kpoint->coord.at(0) = boost::lexical_cast<double>(str_vec_3[5]);
                kpoint->coord.at(1) = boost::lexical_cast<double>(str_vec_3[6]);
                kpoint->coord.at(2) = boost::lexical_cast<double>(str_vec_3[7]);
                kpoint->spin = (boost::format("%1%-%2%")%str_vec_3[3]%boost::replace_all_copy(boost::to_lower_copy(str_vec_3[4]), ":", "")).str();
                kpoint->band.set_size(1, kpoint_set->num_bands);
                kpoint->energy.set_size(1, kpoint_set->num_bands);
                kpoint->occupation.set_size(1, kpoint_set->num_bands);
                for (int iband = 0; iband < kpoint_set->num_bands; iband++) {
                    boost::split(str_vec_4, bands_lines[i+3+k*(kpoint_set->num_bands+2)+2+iband], boost::is_any_of(" "), boost::token_compress_on);
                    kpoint->band.at(iband) = boost::lexical_cast<double>(str_vec_4[1]);
                    kpoint->energy.at(iband) = boost::lexical_cast<double>(str_vec_4[2]);
                    kpoint->occupation.at(iband) = boost::lexical_cast<double>(str_vec_4[3]);
                }
            }
        }
    }

    // build kcoords_1d
    std::vector<double> kcoords_1d;
    kcoords_1d.push_back(0);
    for (int k = 1; k < kpoint_sets[0]->kpoint.size()/(num_spins); k++) {
        auto& kpoint2 = kpoint_sets[0]->kpoint[num_spins*k];
        auto& kpoint1 = kpoint_sets[0]->kpoint[num_spins*(k-1)];
        arma::rowvec point2 = kpoint2->coord.at(0) * cell_k.row(0) + kpoint2->coord.at(1) * cell_k.row(1) + kpoint2->coord.at(2) * cell_k.row(2);
        arma::rowvec point1 = kpoint1->coord.at(0) * cell_k.row(0) + kpoint1->coord.at(1) * cell_k.row(1) + kpoint1->coord.at(2) * cell_k.row(2);
        double distance_in_k_space = arma::norm(point2 - point1);
        double step = distance_in_k_space;
        kcoords_1d.push_back(kcoords_1d[kcoords_1d.size()-1] + step);
    }
    for (int i = 1; i < kpoint_sets.size(); i++) {
        kcoords_1d.push_back(kcoords_1d[kcoords_1d.size()-1]);
        for (int k = 1; k < kpoint_sets[0]->kpoint.size()/(num_spins); k++) {
            auto& kpoint2 = kpoint_sets[i]->kpoint[num_spins*k];
            auto& kpoint1 = kpoint_sets[i]->kpoint[num_spins*(k-1)];
            arma::rowvec point2 = kpoint2->coord.at(0) * cell_k.row(0) + kpoint2->coord.at(1) * cell_k.row(1) + kpoint2->coord.at(2) * cell_k.row(2);
            arma::rowvec point1 = kpoint1->coord.at(0) * cell_k.row(0) + kpoint1->coord.at(1) * cell_k.row(1) + kpoint1->coord.at(2) * cell_k.row(2);
            double distance_in_k_space = arma::norm(point2 - point1);
            double step = distance_in_k_space;
            kcoords_1d.push_back(kcoords_1d[kcoords_1d.size()-1] + step);
        }
    }

    std::vector<double> xtics_locs;
    std::vector<std::string> xtics_labels;
    xtics_locs.push_back(0);
    for (int i = 1; i < kcoords_1d.size(); i++) {
        if (std::abs(kcoords_1d[i] - kcoords_1d[i-1]) < 1.0e-5) {
            xtics_locs.push_back(kcoords_1d[i]);
        }
    }
    xtics_locs.push_back(kcoords_1d[kcoords_1d.size()-1]);

    xtics_labels.push_back(
        kpoint_sets[0]->special_label_1 == "GAMMA" ? "{/symbol G}" : kpoint_sets[0]->special_label_1
    );
    xtics_labels.push_back(
        kpoint_sets[0]->special_label_1 == "GAMMA" ? "{/symbol G}" : kpoint_sets[0]->special_label_1
    );
    for (int i = 1; i < kpoint_sets.size(); i++) {
        if (kpoint_sets[i]->special_label_1 == kpoint_sets[i-1]->special_label_2) {
            // doing nothing
        } else {
            xtics_labels[xtics_labels.size()-1] = kpoint_sets[i-1]->special_label_2 + "|" + (kpoint_sets[i]->special_label_1 == "GAMMA" ? "{/symbol G}" : kpoint_sets[i]->special_label_1);
        }
        xtics_labels.push_back(kpoint_sets[i]->special_label_2 == "GAMMA" ? "{/symbol G}" : kpoint_sets[i]->special_label_2);
    }

    for (int spin = 0; spin < num_spins; spin++) {
        out.open((fs::path(directory) / "post.dir" / (boost::format("band-spin-%1%.data")%(spin+1)).str()).string());
        for (int iband = 0; iband < num_bands; iband++) {
            int kcoord_i = 0;
            for (int iset = 0; iset < kpoint_sets.size(); iset++) {
                for (int k = 0; k < kpoint_sets[iset]->num_kpoints; k++) {
                    out << boost::format("%1$10.6f %2$10.6f\n")
                        % kcoords_1d[kcoord_i]
                        % kpoint_sets[iset]->kpoint[k*num_spins+spin]->energy.at(iband);
                    kcoord_i += 1;
                }
            }
            out << "\n";
        }
        out.close();
    }

    out.open((fs::path(directory) / run_params["post-dir"] / "band.gnuplot").string());
    out << "set terminal pngcairo enhanced\n";
    out << "set parametric\n";
    out << "set title 'Band structure' font ',15'\n";
    out << "set ylabel 'Energy (eV)' font ',15'\n";
    out << "set ytics font ',15'\n";
    out << "set xtics font ',15'\n";
    out << "set border linewidth 3\n";
    out << "set autoscale\n";
    out << "set yrange [-5:5]\n";

    out << "set xtics(";
    for (int i = 0; i < xtics_labels.size()-1; i++) {
        out << boost::format("\'%1%\' %2%, ") % xtics_labels[i] % xtics_locs[i];
    }
    out << boost::format("\'%1%\' %2%)\n") % xtics_labels[xtics_labels.size()-1] % xtics_locs[xtics_locs.size()-1];
    
    for (auto& x : xtics_locs) {
        out << boost::format("set arrow from %1%, graph 0 to %1%, graph 1 nohead linecolor rgb \'black\' linewidth 0.5\n") % x;
    }
    out << boost::format("set arrow from 0, 0 to %1%, 0 nohead linecolor rgb \'black\' linewidth 0.5\n") % xtics_locs[xtics_locs.size()-1];
    
    out << "set output 'band-structure.png'\n";
    out << "plot \\\n";
    for (int spin = 0; spin < num_spins; spin++) {
        if (spin == 0) {            
            out << boost::format("  \'band-spin-%1%.data\' using 1:(column(2)-(%2%)) w l title 'spin-%3%' linecolor rgb \'black\' linewidth 3 linetype 1,\\\n")
                % (spin+1)
                % fermi_in_ev
                % (spin+1);
        } else {
            out << boost::format("  \'band-spin-%1%.data\' using 1:(column(2)-(%2%)) w l title 'spin-%3%' linecolor rgb \'red\' linewidth 3 linetype 1\n")
                % (spin+1)
                % fermi_in_ev
                % (spin+1);
        }
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

} // namespace atomsciflow::cp2k::post
