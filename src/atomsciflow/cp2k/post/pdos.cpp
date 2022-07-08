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

namespace atomsciflow::cp2k::post {

namespace ba = boost::algorithm;

ElementPdos::ElementPdos(const std::string& pdos_file) : ha_to_ev{27.211324570273} {
        this->pdos_file = pdos_file;
        this->kind = "";
        this->fermi = 0; // in a.u.
        this->get_data();
        this->spin = this->get_spin();
}

void ElementPdos::get_data() {

    std::string line;
    std::vector<std::string> vec_str;

    std::ifstream stream;
    stream.open(this->pdos_file);

    std::getline(stream, line);
    ba::replace_all(line, "\t", " ");
    boost::split(vec_str, line, boost::is_any_of(" "), boost::token_compress_on);
    this->kind = vec_str[6];

    std::getline(stream, line);
    ba::replace_all(line, "\\t", " ");
    boost::split(vec_str, line, boost::is_any_of(" "), boost::token_compress_on);
    int i = -1;
    for (auto& item : vec_str) {
        i++;
        if (i < 5) {
            continue;
        }
        this->orbitals.push_back(item);
    }

    this->data.load(stream);

    stream.close();

    // shift the Fermi energy to zero and conver the unit of energy
    // from a.u. to eV
    data.col(1) = data.col(1) - this->fermi;
    data.col(1) = data.col(1) * this->ha_to_ev;
}

std::string ElementPdos::get_spin() {
    std::string spin = "unpolarized";
    std::string filename = fs::path(this->pdos_file).filename().string();
    std::regex pat_alpha("[a-zA-Z\\_*\\-*0-9]*-ALPHA_k\\d+-1\\.pdos$");
    std::regex pat_beta("[a-zA-Z\\_*\\-*0-9]*-BETA_k\\d+-1\\.pdos$");
    std::smatch m1;
    std::smatch m2;
    if (std::regex_search(filename, m1, pat_alpha)) {
        spin = "polarized-alpha";
    } else if (std::regex_search(filename, m2, pat_beta)) {
        spin = "polarized-beta";
    } else {
        spin = "unpolarized";
    }
    return spin;
}

Pdos::Pdos() {

}

Pdos::~Pdos() {

}

void Pdos::run(const std::string& directory) {

    std::vector<std::string> filenames;
    for (auto& filename : fs::directory_iterator(directory)) {
        filenames.push_back(filename.path().string());
    }

    std::regex pat("[a-zA-Z\\_*\\-*0-9]*_k\\d+-1\\.pdos$");
    std::smatch m1;
    for (auto& item : filenames) {
        if (std::regex_search(item, m1, pat)) {
            data.push_back(std::make_shared<ElementPdos>(item));
            data_smearing.push_back(std::make_shared<ElementPdos>(item));
        }
    }

    auto delta = [&](double emin, double emax, int npts, double energy, double width) -> arma::vec {
        auto energies = arma::linspace(emin, emax, npts);
        arma::vec x = - arma::pow((energies - energy) / width, 2);
        return arma::exp(x) / (std::sqrt(arma::datum::pi) * width);
    };

    auto convolute = [&](double width) {
        for (int i = 0; i < data.size(); i++) {
            double emin = arma::min(data[i]->data.col(1));
            double emax = arma::max(data[i]->data.col(1));
            for (int j = 3; j < data[i]->data.n_cols; j++) {
                data_smearing[i]->data.col(j).zeros();
                for (int k = 0; k < data[i]->data.n_rows; k++) {
                    auto tmp_vec = delta(emin, emax, data[i]->data.n_rows, data[i]->data.at(k, 1), width);
                    data_smearing[i]->data.col(j) += data[i]->data.at(k, j) * tmp_vec;
                }
            }
            data_smearing[i]->data.col(1) = arma::linspace(emin, emax, data_smearing[i]->data.n_rows);
        }
    };

    convolute(0.8);

    // export smearing pdos data
    fs::create_directory(fs::path(directory) / run_params["post-dir"]);
    
    std::ofstream stream;
    for (int i = 0; i < data_smearing.size(); i++) {
        stream.open((fs::path(directory) / run_params["post-dir"] / fs::path(data_smearing[i]->pdos_file).replace_extension(".pdos.smearing.data").filename()).string());
        stream << "#MO\tEigenvalue[eV]\tOccupation";
        for (auto& orb : data_smearing[i]->orbitals) {
            stream << "\t" << orb;
        }
        stream << "\ttotal(" << data_smearing[i]->kind << ")";
        stream << "\n";
        stream << "#int\teV\tfloat";
        for (auto& orb : data_smearing[i]->orbitals) {
            stream << "\tfloat";
        }
        stream << "\tfloat"; 
        stream << "\n";
        stream << "#Projected DOS for the atomic kind " << data_smearing[i]->kind << ",  eigenvalue are transferred from a.u. to eV already and efermi shifted to 0\n";
        for (int j = 0; j < data_smearing[i]->data.n_rows; j++) {
            stream << data_smearing[i]->data.at(j, 0);
            stream << "\t" << data_smearing[i]->data.at(j, 1) << "\t" << data_smearing[i]->data.at(j, 2);
            for (int k = 3; k < data_smearing[i]->data.n_cols; k++) {
                stream << "\t" << data_smearing[i]->data.at(j, k);
            }
            stream << "\t" << arma::sum(data_smearing[i]->data.row(j)) 
                - data_smearing[i]->data.at(j, 0)
                - data_smearing[i]->data.at(j, 1)
                - data_smearing[i]->data.at(j, 2);
            stream << "\n";
        }
        stream.close();
    }

    stream.open((fs::path(directory) / run_params["post-dir"] / "element-projected.pdos.smearing.data").string());
    stream << "#MO\tEigenvalue[eV]";
    for (int i = 0; i < data_smearing.size(); i++) {
        if (boost::contains(data_smearing[i]->spin, "alpha")) {
            stream << boost::format("\t%1%(alpha)") % data_smearing[i]->kind;
        } else if (boost::contains(data_smearing[i]->spin, "beta")) {
            stream << boost::format("\t%1%(beta)") % data_smearing[i]->kind;
        } else {
            stream << boost::format("\t%1%") % data_smearing[i]->kind;
        }
    }
    if (data_smearing[0]->spin != "unpolarized") {
        stream << "\tTDOS(alpha)\n";
        stream << "\tTDOS(beta)\n";
    } else {
        stream << "\tTDOS\n";
    }
    stream << "#int\teV";
    for (int i = 0; i < data_smearing.size(); i++) {
        stream << "\tfloat";
    }
    if (data_smearing[0]->spin == "unpolarized") {
        stream << "\tfloat\n";
    } else {
        stream << "\tfloat\n";
        stream << "\tfloat\n";
    }
    stream << "#eigenvalue are transfered from a.u. to eV already and efermi shifted to 0\n";
    for (int j = 0; j < data_smearing[0]->data.n_rows; j++) {
        stream << (j + 1);
        stream << "\t" << data_smearing[0]->data.at(j, 1);
        for (int k = 0; k < data_smearing.size(); k++) {
            stream << "\t" << arma::sum(data_smearing[k ]->data.row(j))
                - data_smearing[k]->data.at(j, 0)
                - data_smearing[k]->data.at(j, 1)
                - data_smearing[k]->data.at(j, 2);
        }
        if (data_smearing[0]->spin == "unpolarized") {
            arma::vec tmp_vec;
            tmp_vec.zeros(data_smearing.size());
            for (int l = 0; l < tmp_vec.size(); l++) {
                for (int o = 0; o < data_smearing[l]->orbitals.size(); o++) {
                    tmp_vec.at(l) += data_smearing[l]->data.at(j, o + 3);
                }
            }
            stream << "\t" << arma::sum(tmp_vec);
        } else {
            arma::vec tmp_vec;
            // alpha
            tmp_vec.zeros(data_smearing.size());
            for (int l = 0; l < tmp_vec.size(); l++) {
                if (data_smearing[l]->spin != "polarized-alpha") {
                    continue;
                }
                for (int o = 0; o < data_smearing[l]->orbitals.size(); o++) {
                    tmp_vec.at(l) += data_smearing[l]->data.at(j, o + 3);
                }
            }
            stream << "\t" << arma::sum(tmp_vec);
            // beta
            tmp_vec.zeros(data_smearing.size());
            for (int l = 0; l < tmp_vec.size(); l++) {
                if (data_smearing[l]->spin != "polarized-beta") {
                    continue;
                }                
                for (int o = 0; o < data_smearing[l]->orbitals.size(); o++) {
                    tmp_vec.at(l) += data_smearing[l]->data.at(j, o + 3);
                }
            }
            stream << "\t" << arma::sum(tmp_vec);
        }
        stream << "\n";
    }
    stream.close();

    stream.open((fs::path(directory) / run_params["post-dir"] / "plot.gnuplot").string());
    stream << "set terminal png\n"
        << "set parametric\n"
        << "set title 'Element Projected Density of States'\n"
        << "set xlabel 'Energy(eV)'\n"
        << "set ylabel 'PDOS'\n"
        << "set grid xtics ytics\n"
        << "set autoscale\n"
        << "set xrange [-10:10]\n";
        if (data_smearing[0]->spin == "unpolarized") {
            stream << "set output 'element-projected-pdos.png'\n"
                << "plot \\\n";
            for (int i = 0; i < data_smearing.size(); i++) {
                stream << boost::format("'element-projected.pdos.smearing.data' using 2:%1% title '%2%' w l, \\\n")
                    % (i+3) % (i+3);
            }
            stream << boost::format("'element-projected.pdos.smearing.data' using 2:%1% title '%2%' w l, \\\n")
                % (data_smearing.size()+3) % "Total";
        } else {
            stream << "set output 'element-projected-pdos.png'\n"
                << "plot \\\n";
            for (int i = 0; i < data_smearing.size(); i++) {
                if (boost::contains(data_smearing[i]->spin, "alpha")) {
                    stream << boost::format("'element-projected.pdos.smearing.data' using 2:%1% title '%2%(alpha)' w l, \\\n")
                        % (i+3) % data_smearing[i]->kind;
                } else if (boost::contains(data_smearing[i]->spin, "beta")) {
                    stream << boost::format("'element-projected.pdos.smearing.data' using 2:(-$%1%) title '%2%(beta)' w l, \\\n")
                        % (i+3) % data_smearing[i]->kind;
                }
            }
            stream << boost::format("'element-projected.pdos.smearing.data' using 2:%1% title '%2%' w l, \\\n")
                % (data_smearing.size()+3) % "Total(alpha)";
            stream << boost::format("'element-projected.pdos.smearing.data' using 2:(-$%1%) title '%2%' w l, \\\n")
                % (data_smearing.size()+3+1) % "Total(beta)";
        }
    stream.close();

    stream.open((fs::path(directory) / run_params["post-dir"] / "analysis.sh").string());
    stream << "#!/bin/bash\n\n"
        << "#\n"
        << boost::format("cd %1%\n") % (fs::absolute(fs::path(directory)) / run_params["post-dir"]).string()
        << "\n"
        << "gnuplot plot.gnuplot\n";
    stream.close();

    std::string cmd = "";
    cmd += "bash ";
    cmd += (fs::path(directory) / run_params["post-dir"] / "analysis.sh").string();
    std::system(cmd.c_str());
}

} // namespace atomsciflow::cp2k::post
