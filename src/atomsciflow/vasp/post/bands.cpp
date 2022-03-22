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

namespace atomsciflow {

/// \brief PostBands::get_efermi
/// \param vasprun
/// Extract fermi energy from the vasprun.xml of either scf
/// or nscf output.
void PostBands::get_efermi(std::string vasprun="vasprun.xml") {
    pt::ptree vasprun_root;
    pt::read_xml(vasprun, vasprun_root);
    this->efermi = vasprun_root.get<double>("calculation.dos.i");
}

void PostBands::get_kpath_and_vasprun(Kpath kpath, std::string vasprun) {

    pt::read_xml(vasprun, this->vasprun_root);

    this->kpath = kpath;

    this->get_xcoord_k();
    this->get_eigenval();

    this->locs.emplace_back(this->xcoord_k[0]);
    for (int i = 1; i < this->xcoord_k.size() - 1; i++) {
        if (this->xcoord_k[i] == this->xcoord_k[i-1]) {
            this->locs.emplace_back(this->xcoord_k[i]);
        }
    }
    this->locs.emplace_back(this->xcoord_k[-1]);

    this->labels_for_gnuplot.emplace_back(
        ba::to_upper_copy(this->kpath.labels[0]) != "GAMMA" ? ba::to_upper_copy(this->kpath.labels[0]) : "{/symbol G}"
    );
    for (int i = 1; i < this->kpath.coords.size(); i++) {
        if (this->kpath.links[i-1] != 0) {
            this->labels_for_gnuplot.emplace_back(
                ba::to_upper_copy(this->kpath.labels[i]) != "GAMMA" ? this->kpath.labels[i] : "{/symbol G}"
            );
        } else {
            this->labels_for_gnuplot[-1] = this->labels_for_gnuplot[-1] + " | " + ba::to_upper_copy(this->kpath.labels[i]);
        }
    }
}

void PostBands::get_xcoord_k() {
    std::vector<std::vector<double>> cell;
    std::vector<double> vec;
    std::string tmp_str;
    std::vector<std::string> str_vec;
    std::vector<pt::ptree::value_type> pt_vec;
    std::vector<pt::ptree::value_type> pt_vec_1;
    std::vector<pt::ptree::value_type> pt_vec_2;

    vec.clear();
    pt_vec.clear();
    pt_vec_1.clear();
    pt_vec_2.clear();
    for (const auto& item : this->vasprun_root.get_child("modeling").get_child("structure") ) {
        pt_vec.emplace_back(item);
    }
    for (const auto& item : pt_vec[-1].second.get_child("crystal").get_child("varray")) {
        pt_vec_1.emplace_back(item);
    }
    for (const auto& item : pt_vec_1[0].second.get_child("v") ) {
        pt_vec_2.emplace_back(item);
    }
    tmp_str = pt_vec_2[0].second.get_value<std::string>();
    boost::split(str_vec, tmp_str, boost::is_space());
    vec.emplace_back(boost::lexical_cast<double>(str_vec[0]));
    vec.emplace_back(boost::lexical_cast<double>(str_vec[1]));
    vec.emplace_back(boost::lexical_cast<double>(str_vec[2]));
    cell.push_back(vec);

    vec.clear();
    pt_vec.clear();
    pt_vec_1.clear();
    pt_vec_2.clear();
    for (const auto& item : this->vasprun_root.get_child("modeling").get_child("structure") ) {
        pt_vec.emplace_back(item);
    }
    for (const auto& item : pt_vec[-1].second.get_child("crystal").get_child("varray")) {
        pt_vec_1.emplace_back(item);
    }
    for (const auto& item : pt_vec_1[0].second.get_child("v") ) {
        pt_vec_2.emplace_back(item);
    }
    tmp_str = pt_vec_2[1].second.get_value<std::string>();
    boost::split(str_vec, tmp_str, boost::is_space());
    vec.emplace_back(boost::lexical_cast<double>(str_vec[0]));
    vec.emplace_back(boost::lexical_cast<double>(str_vec[1]));
    vec.emplace_back(boost::lexical_cast<double>(str_vec[2]));
    cell.push_back(vec);

    vec.clear();
    pt_vec.clear();
    pt_vec_1.clear();
    pt_vec_2.clear();
    for (const auto& item : this->vasprun_root.get_child("modeling").get_child("structure") ) {
        pt_vec.emplace_back(item);
    }
    for (const auto& item : pt_vec[-1].second.get_child("crystal").get_child("varray")) {
        pt_vec_1.emplace_back(item);
    }
    for (const auto& item : pt_vec_1[0].second.get_child("v") ) {
        pt_vec_2.emplace_back(item);
    }
    tmp_str = pt_vec_2[2].second.get_value<std::string>();
    boost::split(str_vec, tmp_str, boost::is_space());
    vec.emplace_back(boost::lexical_cast<double>(str_vec[0]));
    vec.emplace_back(boost::lexical_cast<double>(str_vec[1]));
    vec.emplace_back(boost::lexical_cast<double>(str_vec[2]));
    cell.push_back(vec);

    double a1 = arma::norm(arma::conv_to<arma::vec>::from(cell[0]));
    double a2 = arma::norm(arma::conv_to<arma::vec>::from(cell[1]));
    double a3 = arma::norm(arma::conv_to<arma::vec>::from(cell[2]));

    std::vector<std::vector<double>> cell_b_vasp;

    vec.clear();
    pt_vec.clear();
    pt_vec_1.clear();
    pt_vec_2.clear();
    for (const auto& item : this->vasprun_root.get_child("modeling").get_child("structure") ) {
        pt_vec.emplace_back(item);
    }
    for (const auto& item : pt_vec[-1].second.get_child("crystal").get_child("varray")) {
        pt_vec_1.emplace_back(item);
    }
    for (const auto& item : pt_vec_1[1].second.get_child("v") ) {
        pt_vec_2.emplace_back(item);
    }
    tmp_str = pt_vec_2[0].second.get_value<std::string>();
    boost::split(str_vec, tmp_str, boost::is_space());
    vec.emplace_back(boost::lexical_cast<double>(str_vec[0]));
    vec.emplace_back(boost::lexical_cast<double>(str_vec[1]));
    vec.emplace_back(boost::lexical_cast<double>(str_vec[2]));
    cell_b_vasp.push_back(vec);

    vec.clear();
    pt_vec.clear();
    pt_vec_1.clear();
    pt_vec_2.clear();
    for (const auto& item : this->vasprun_root.get_child("modeling").get_child("structure") ) {
        pt_vec.emplace_back(item);
    }
    for (const auto& item : pt_vec[-1].second.get_child("crystal").get_child("varray")) {
        pt_vec_1.emplace_back(item);
    }
    for (const auto& item : pt_vec_1[1].second.get_child("v") ) {
        pt_vec_2.emplace_back(item);
    }
    tmp_str = pt_vec_2[1].second.get_value<std::string>();
    boost::split(str_vec, tmp_str, boost::is_space());
    vec.emplace_back(boost::lexical_cast<double>(str_vec[0]));
    vec.emplace_back(boost::lexical_cast<double>(str_vec[1]));
    vec.emplace_back(boost::lexical_cast<double>(str_vec[2]));
    cell_b_vasp.push_back(vec);

    vec.clear();
    pt_vec.clear();
    pt_vec_1.clear();
    pt_vec_2.clear();
    for (const auto& item : this->vasprun_root.get_child("modeling").get_child("structure") ) {
        pt_vec.emplace_back(item);
    }
    for (const auto& item : pt_vec[-1].second.get_child("crystal").get_child("varray")) {
        pt_vec_1.emplace_back(item);
    }
    for (const auto& item : pt_vec_1[1].second.get_child("v") ) {
        pt_vec_2.emplace_back(item);
    }
    tmp_str = pt_vec_2[2].second.get_value<std::string>();
    boost::split(str_vec, tmp_str, boost::is_space());
    vec.emplace_back(boost::lexical_cast<double>(str_vec[0]));
    vec.emplace_back(boost::lexical_cast<double>(str_vec[1]));
    vec.emplace_back(boost::lexical_cast<double>(str_vec[2]));
    cell_b_vasp.push_back(vec);

    arma::mat latcell(3, 3);
    latcell.row(0) = arma::conv_to<arma::rowvec>::from(cell[0]);
    latcell.row(1) = arma::conv_to<arma::rowvec>::from(cell[1]);
    latcell.row(2) = arma::conv_to<arma::rowvec>::from(cell[2]);

    double V = arma::dot(latcell.row(0), arma::cross(latcell.row(1), latcell.row(2)));
    arma::vec b1_vec = arma::cross(latcell.row(1), latcell.row(2)) * 2 * arma::datum::pi / V;
    arma::vec b2_vec = arma::cross(latcell.row(2), latcell.row(0)) * 2 * arma::datum::pi / V;
    arma::vec b3_vec = arma::cross(latcell.row(0), latcell.row(1)) * 2 * arma::datum::pi / V;

    double b1 = arma::norm(b1_vec);
    double b2 = arma::norm(b2_vec);
    double b3 = arma::norm(b3_vec);

    std::cout << "cell a:";
    std::cout << cell[0][0] << " " << cell[0][1] << " " << cell[0][2] << "\n";
    std::cout << cell[1][0] << " " << cell[1][1] << " " << cell[1][2] << "\n";
    std::cout << cell[2][0] << " " << cell[2][1] << " " << cell[2][2] << "\n";
    std::cout << "cell b:\n";
    std::cout << b1_vec[0] << " " << b1_vec[1] << " " << b1_vec[2] << "\n";
    std::cout << b2_vec[0] << " " << b2_vec[1] << " " << b2_vec[2] << "\n";
    std::cout << b3_vec[0] << " " << b3_vec[1] << " " << b3_vec[2] << "\n";

    this->xcoord_k.push_back(0.0000000);
    for (int i = 0; i < this->kpath.coords.size(); i++) {
        if (this->kpath.links[i-1] != 0) {
            auto vec1 = this->kpath.coords[i-1][0] * b1_vec + this->kpath.coords[i-1][1] * b2_vec + this->kpath.coords[i-1][2] * b3_vec;
            auto vec2 = this->kpath.coords[i][0] * b1_vec + this->kpath.coords[i][1] * b2_vec + this->kpath.coords[i][2] * b3_vec;
            double distance_in_b = arma::norm(vec2-vec1);
            double step = distance_in_b / (this->kpath.links[i-1] - 1);

            for (int j = 0; j < this->kpath.links[i-1]-1; j ++) {
                this->xcoord_k.push_back(this->xcoord_k[-1]+step);
            }
            if (i == this->kpath.coords.size() - 1) {
                continue;
            } else {
                this->xcoord_k.push_back(this->xcoord_k[-1]);
            }
        }
    }
}

void PostBands::get_eigenval() {
}

void PostBands::_plot_band_gnuplot(std::vector<double>& bandrange, std::vector<double>& xrange, std::vector<double>& yrange) {
}

void PostBands::process(std::string directory, std::vector<double>& bandrange, std::vector<double>& xrange, std::vector<double>& yrange) {
}

} // namespace atomsciflow
