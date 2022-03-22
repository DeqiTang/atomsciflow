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

/// @file src/atomsciflow/utils/structure.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Wed 09 Mar 2022 08:06:59 PM CST

#include "atomsciflow/utils/structure.h"

#include <armadillo>

namespace atomsciflow {

/// crystal/fractional coordinate can be convert from cartesian coordinates
/// the conversion process is like transformation of presentation in quantum
/// mechanics the convmat is bulid to do the conversion
void atoms_cart_to_frac(std::vector<Atom>& atoms_frac, const std::vector<Atom>& atoms_cart, const std::vector<std::vector<double>>& cell) {

    atoms_frac.clear();

    arma::mat latcell(3, 3);
    latcell.row(0) = arma::conv_to<arma::rowvec>::from(cell[0]);
    latcell.row(1) = arma::conv_to<arma::rowvec>::from(cell[1]);
    latcell.row(2) = arma::conv_to<arma::rowvec>::from(cell[2]);
    arma::mat convmat = arma::inv(arma::trans(latcell));

    arma::vec cart_tmp(3);
    arma::vec frac_tmp(3);

    for (int i = 0; i < atoms_cart.size(); i++) {
        cart_tmp[0] = atoms_cart[i].x;
        cart_tmp[1] = atoms_cart[i].y;
        cart_tmp[2] = atoms_cart[i].z;
        //frac_tmp = arma::dot(convmat, arma::vec(cart_tmp));
        frac_tmp = convmat * arma::vec(cart_tmp);
        Atom atom;
        atom.x = frac_tmp[0];
        atom.y = frac_tmp[1];
        atom.z = frac_tmp[2];
        atoms_frac.emplace_back(atom);
    }
    return;
}

void atoms_frac_to_cart(std::vector<Atom>& atoms_cart, const std::vector<Atom>& atoms_frac, const std::vector<std::vector<double>>& cell) {

    atoms_cart.clear();

    arma::mat latcell(3, 3);
    latcell.row(0) = arma::conv_to<arma::rowvec>::from(cell[0]);
    latcell.row(1) = arma::conv_to<arma::rowvec>::from(cell[1]);
    latcell.row(2) = arma::conv_to<arma::rowvec>::from(cell[2]);
    arma::mat convmat = arma::trans(latcell);

    arma::vec cart_tmp(3);
    arma::vec frac_tmp(3);

    for (int i = 0; i < atoms_frac.size(); i++) {
        frac_tmp[0] = atoms_frac[i].x;
        frac_tmp[1] = atoms_frac[i].y;
        frac_tmp[2] = atoms_frac[i].z;
        //cart_tmp = arma::dot(convmat, frac_tmp);
        cart_tmp = convmat * frac_tmp;
        Atom atom;
        atom.x = cart_tmp[0];
        atom.y = cart_tmp[1];
        atom.z = cart_tmp[2];
        atoms_cart.emplace_back(atom);
    }
    return;
}

} // namespace atomsciflow
