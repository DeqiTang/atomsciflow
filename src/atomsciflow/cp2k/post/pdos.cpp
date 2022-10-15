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

#include "atomsciflow/cp2k/post/pdos.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>

namespace atomsciflow::cp2k::post {

namespace ba = boost::algorithm;

Pdos::Pdos() {
    this->set_run("program-out", "cp2k.out");
    this->set_run("output-json", "post-dos.json");

    this->smear_width = 0.01;
    this->ha_to_ev = 27.211324570273;
}

Pdos::~Pdos() {

}

/**
 * @brief 
 * 
 * if FORCE_EVAL/DFT/PRINT/PDOS/COMPONENTS is set to .FALSE.(dfault)
 * the pdos will be print out, projected to angular momentum l components(e.g. s p d f).
 * when it is set to .TRUE. the pdos will  be projected to angular momentum l
 * and magnetic quantum number m components(e.g. px py pz)
 * @param directory 
 */
void Pdos::run(const std::string& directory) {
    Post::run(directory);

    arma::rowvec energies;
    double fermi_in_ev;
    std::vector<std::string> str_vec;
    std::string line;

    double smearing_width = this->smear_width / this->ha_to_ev;


    auto delta = [&](double emin, double emax, int npts, double energy, double width) -> arma::vec {
        auto energies = arma::linspace(emin, emax, npts);
        arma::vec x = - arma::pow((energies - energy) / width, 2);
        return arma::exp(x) / (std::sqrt(arma::datum::pi) * width);
    };

    std::ifstream fin;
    std::ofstream out;
    std::regex pat_cp2k_pdos("k\\d+-1\\.pdos$");
    std::smatch match_cp2k_pdos;
    for (auto& item : fs::directory_iterator(directory)) {
        std::string filename = item.path().filename().string();
        if (std::regex_search(filename, match_cp2k_pdos, pat_cp2k_pdos)) {
            fin.open((fs::path(directory) / filename).string());
            std::getline(fin, line);
            ba::replace_all(line, "\t", " ");
            boost::split(str_vec, line, boost::is_any_of(" "), boost::token_compress_on);
            std::string element = str_vec[6];
            fermi_in_ev = boost::lexical_cast<double>(str_vec[15]) * ha_to_ev;            
            std::getline(fin, line);
            boost::split(str_vec, line, boost::is_any_of(" "), boost::token_compress_on);
            std::vector<std::string> components;
            for (int i = 5; i < str_vec.size(); i++) {
                components.push_back(str_vec[i]);
            }
            arma::mat data;
            data.load(fin);
            fin.close();
            arma::mat smearing_data = arma::zeros(data.n_rows, data.n_cols);
            smearing_data.col(0) = data.col(0);
            smearing_data.col(1) = data.col(1);
            smearing_data.col(2) = data.col(2);
            // smearing the data
            double emin = arma::min(data.col(1));
            double emax = arma::max(data.col(1));
            for (int j = 3; j < data.n_cols; j++) {
                smearing_data.col(j).zeros();
                for (int k = 0; k < data.n_rows; k++) {
                    auto tmp_vec = delta(emin, emax, data.n_rows, data.at(k, 1), smearing_width);
                    smearing_data.col(j) += data.at(k, j) * tmp_vec;
                }
            }
            smearing_data.col(1) = arma::linspace(emin, emax, smearing_data.n_rows);
            smearing_data.col(1) *= ha_to_ev;
            energies = arma::conv_to<arma::rowvec>::from(smearing_data.col(1));
            // output smearing data
            std::string spin = "unpolarized";
            std::regex pat_alpha("[a-zA-Z\\_*\\-*0-9]*-ALPHA_k\\d+-1\\.pdos$");
            std::regex pat_beta("[a-zA-Z\\_*\\-*0-9]*-BETA_k\\d+-1\\.pdos$");
            std::smatch m1;
            std::smatch m2;
            if (std::regex_search(filename, m1, pat_alpha)) {
                spin = "alpha";
            } else if (std::regex_search(filename, m2, pat_beta)) {
                spin = "beta";
            } else {
                spin = "unpolarized";
            }            
            out.open((fs::path(directory) / "post.dir" / (boost::format("pdos-%1%-%2%.data")%element%spin).str()).string());
            out << boost::format("# data extracted and smeared from %1%, fermi energy(eV): %2%\n") % filename % fermi_in_ev;
            int n_components = smearing_data.n_cols - 3;
            out << boost::format("# MO Eigenvalue [eV] Occupation");
            for (auto& component : components) {
                out << boost::format(" %1%") % component;
            }
            out << "\n";
            for (int i = 0; i < smearing_data.n_rows; i++) {
                out << boost::format("%1% %2% %3$10.6f") 
                    % smearing_data.at(i, 0)
                    % smearing_data.at(i, 1)
                    % smearing_data.at(i, 2);
                for (int l = 0; l < n_components; l++) {
                    out << boost::format(" %1%") % smearing_data.at(i, l+3);
                }
                out << "\n";
            }
            out.close();
        }
    }

    std::map<std::string, arma::rowvec> elem_proj;
    std::regex pat_smearing_pdos("pdos-[A-Za-z]{1,2}-[A-Za-z]+\\.data$");
    std::smatch match_smearing_pdos;
    for (auto& item : fs::directory_iterator((fs::path(directory)/"post.dir").string())) {
        std::string filename = item.path().filename().string();
        if (std::regex_search(filename, match_smearing_pdos, pat_smearing_pdos)) {
            boost::split(str_vec, filename, boost::is_any_of("-"), boost::token_compress_on);
            std::string element = str_vec[1];
            std::string spin = boost::replace_all_copy(str_vec[2], ".data", "");
            std::string key = (boost::format("%1%-%2%")%element%spin).str();
            arma::mat data;
            fin.open((fs::path(directory) / "post.dir" / filename).string());
            std::getline(fin, line);
            std::getline(fin, line);
            data.load(fin);
            fin.close();
            elem_proj[key] = arma::conv_to<arma::rowvec>::from(arma::zeros(data.n_rows));
            for (int j = 3; j < data.n_cols; j++) {
                elem_proj[key] += arma::conv_to<arma::rowvec>::from(data.col(j));
            }
        }
    }

    for (auto& item : elem_proj) {
        out.open((fs::path(directory) / "post.dir" / (boost::format("elem-proj-%1%.data")%item.first).str()).string());
        for (int i = 0; i < energies.size(); i++) {
            out << boost::format("%1$10.6f %2$10.6f\n") % energies.at(i) % item.second.at(i);
        }
        out.close();
    }

    std::map<std::string, arma::rowvec> elem_component_proj;
    for (auto& item : fs::directory_iterator((fs::path(directory)/"post.dir").string())) {
        std::string filename = item.path().filename().string();
        if (std::regex_search(filename, match_smearing_pdos, pat_smearing_pdos)) {
            boost::split(str_vec, filename, boost::is_any_of("-"), boost::token_compress_on);
            std::string element = str_vec[1];
            std::string spin = boost::replace_all_copy(str_vec[2], ".data", "");

            arma::mat data;
            fin.open((fs::path(directory) / "post.dir" / filename).string());
            std::getline(fin, line);
            std::getline(fin, line);
            boost::split(str_vec, line, boost::is_any_of(" "), boost::token_compress_on);
            std::vector<std::string> components;
            for (int i = 5; i < str_vec.size(); i++) {
                components.push_back(str_vec[i]);
            }            
            data.load(fin);
            fin.close();

            for (int j = 3; j < data.n_cols; j++) {
                std::string key = (boost::format("%1%-%2%-%3%")%element%components[j-3]%spin).str();
                elem_component_proj[key] = arma::conv_to<arma::rowvec>::from(data.col(j));
            }
        }
    }

    for (auto& item : elem_component_proj) {
        out.open((fs::path(directory) / "post.dir" / (boost::format("elem-component-proj-%1%.data")%item.first).str()).string());
        for (int i = 0; i < energies.size(); i++) {
            out << boost::format("%1$10.6f %2$10.6f\n") % energies.at(i) % item.second.at(i);
        }
        out.close();
    }

    out.open((fs::path(directory) / "post.dir/dos.gnuplot").string());
    out << boost::format("efermi = %1%\n") % fermi_in_ev;
    out << "set terminal pngcairo enhanced\n";
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
    out << "set arrow from -20, 0 to 20, 0 nohead linecolor rgb \'black\' linewidth 0.5\n";
    
    int i = 0;

    out << "set output \'elem-proj.png\'\n";
    out << "plot \\\n";        
    i = 0;
    for (auto& item : elem_proj) {
        if (i != elem_proj.size() - 1) {
            out << boost::format("  \'elem-proj-%1%.data\' using (column(1)-(efermi)):(column(2)*(%2%)) w l title \'%3%\' linewidth 3,\\\n")
                % item.first
                % (boost::contains(item.first, "beta") ? -1 : 1)
                % item.first;
        } else {
            out << boost::format("  \'elem-proj-%1%.data\' using (column(1)-(efermi)):(column(2)*(%2%)) w l title \'%3%\' linewidth 3\n")
                % item.first 
                % (boost::contains(item.first, "beta") ? -1 : 1)
                % item.first;
        }
        i++;
    }

    out << "set output \'elem-component-proj.png\'\n";
    out << "plot \\\n";        
    i = 0;
    for (auto& item : elem_component_proj) {
        if (i != elem_component_proj.size() - 1) {
            out << boost::format("  \'elem-component-proj-%1%.data\' using (column(1)-(efermi)):(column(2)*(%2%)) w l title \'%3%\' linewidth 3,\\\n")
                % item.first
                % (boost::contains(item.first, "beta") ? -1 : 1)
                % item.first;
        } else {
            out << boost::format("  \'elem-component-proj-%1%.data\' using (column(1)-(efermi)):(column(2)*(%2%)) w l title \'%3%\' linewidth 3\n")
                % item.first 
                % (boost::contains(item.first, "beta") ? -1 : 1)
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

} // namespace atomsciflow::cp2k::post
