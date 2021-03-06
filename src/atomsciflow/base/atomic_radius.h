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


/*
-----------------------------------------------------------
Information of the atomic radius from:
* https://en.wikipedia.org/wiki/Atomic_radii_of_the_elements_(data_page)
* https://en.wikipedia.org/wiki/Atomic_radius
-----------------------------------------------------------
*/

#ifndef ATOMSCIFLOW_BASE_ATOMIC_RADIUS_H_
#define ATOMSCIFLOW_BASE_ATOMIC_RADIUS_H_

#include <map>
#include <string>

namespace atomsciflow {

class AtomicRadius {
public:
  AtomicRadius() {
    this->_initialize();
  };
  ~AtomicRadius() {};

  std::map<std::string, double> emprical;
  std::map<std::string, double> calculated;
  std::map<std::string, double> vanderwaals;
  std::map<std::string, double> covalent_single_bond;
  std::map<std::string, double> covalent_triple_bond;

private:
  void _initialize();


};

inline void AtomicRadius::_initialize() {
  this->emprical["H"] = 0.25;
  this->calculated["H"] = 0.53;
  this->vanderwaals["H"] = 1.2;
  this->covalent_single_bond["H;"] = 0.32;
  this->emprical["He"] = 1.2;
  this->calculated["He"] = 0.31;
  this->vanderwaals["He"] = 1.4;
  this->covalent_single_bond["He;"] = 0.46;
  this->emprical["Li"] = 1.45;
  this->calculated["Li"] = 1.67;
  this->vanderwaals["Li"] = 1.82;
  this->covalent_single_bond["Li;"] = 1.33;
  this->emprical["Be"] = 1.05;
  this->calculated["Be"] = 1.12;
  this->vanderwaals["Be"] = 1.53;
  this->covalent_single_bond["Be;"] = 1.02;
  this->covalent_triple_bond["Be;"] = 0.85;
  this->emprical["B"] = 0.85;
  this->calculated["B"] = 0.87;
  this->vanderwaals["B"] = 1.92;
  this->covalent_single_bond["B;"] = 0.85;
  this->covalent_triple_bond["B;"] = 0.73;
  this->emprical["C"] = 0.7;
  this->calculated["C"] = 0.67;
  this->vanderwaals["C"] = 1.7;
  this->covalent_single_bond["C;"] = 0.75;
  this->covalent_triple_bond["C;"] = 0.6;
  this->emprical["N"] = 0.65;
  this->calculated["N"] = 0.56;
  this->vanderwaals["N"] = 1.55;
  this->covalent_single_bond["N;"] = 0.71;
  this->covalent_triple_bond["N;"] = 0.54;
  this->emprical["O"] = 0.6;
  this->calculated["O"] = 0.48;
  this->vanderwaals["O"] = 1.52;
  this->covalent_single_bond["O;"] = 0.63;
  this->covalent_triple_bond["O;"] = 0.53;
  this->emprical["F"] = 0.5;
  this->calculated["F"] = 0.42;
  this->vanderwaals["F"] = 1.47;
  this->covalent_single_bond["F;"] = 0.64;
  this->covalent_triple_bond["F;"] = 0.53;
  this->emprical["Ne"] = 1.6;
  this->calculated["Ne"] = 0.38;
  this->vanderwaals["Ne"] = 1.54;
  this->covalent_single_bond["Ne;"] = 0.67;
  this->emprical["Na"] = 1.8;
  this->calculated["Na"] = 1.9;
  this->vanderwaals["Na"] = 2.27;
  this->covalent_single_bond["Na;"] = 1.55;
  this->emprical["Mg"] = 1.5;
  this->calculated["Mg"] = 1.45;
  this->vanderwaals["Mg"] = 1.73;
  this->covalent_single_bond["Mg;"] = 1.39;
  this->covalent_triple_bond["Mg;"] = 1.27;
  this->emprical["Al"] = 1.25;
  this->calculated["Al"] = 1.18;
  this->vanderwaals["Al"] = 1.84;
  this->covalent_single_bond["Al;"] = 1.26;
  this->covalent_triple_bond["Al;"] = 1.11;
  this->emprical["Si"] = 1.1;
  this->calculated["Si"] = 1.11;
  this->vanderwaals["Si"] = 2.1;
  this->covalent_single_bond["Si;"] = 1.16;
  this->covalent_triple_bond["Si;"] = 1.02;
  this->emprical["P"] = 1.0;
  this->calculated["P"] = 0.98;
  this->vanderwaals["P"] = 1.8;
  this->covalent_single_bond["P;"] = 1.11;
  this->covalent_triple_bond["P;"] = 0.94;
  this->emprical["S"] = 1.0;
  this->calculated["S"] = 0.88;
  this->vanderwaals["S"] = 1.8;
  this->covalent_single_bond["S;"] = 1.03;
  this->covalent_triple_bond["S;"] = 0.95;
  this->emprical["Cl"] = 1.0;
  this->calculated["Cl"] = 0.79;
  this->vanderwaals["Cl"] = 1.75;
  this->covalent_single_bond["Cl;"] = 0.99;
  this->covalent_triple_bond["Cl;"] = 0.93;
  this->emprical["Ar"] = 0.71;
  this->calculated["Ar"] = 0.71;
  this->vanderwaals["Ar"] = 1.88;
  this->covalent_single_bond["Ar;"] = 0.96;
  this->covalent_triple_bond["Ar;"] = 0.96;
  this->emprical["K"] = 2.2;
  this->calculated["K"] = 2.43;
  this->vanderwaals["K"] = 2.75;
  this->covalent_single_bond["K;"] = 1.96;
  this->emprical["Ca"] = 1.8;
  this->calculated["Ca"] = 1.94;
  this->vanderwaals["Ca"] = 2.31;
  this->covalent_single_bond["Ca;"] = 1.71;
  this->covalent_triple_bond["Ca;"] = 1.33;
  this->emprical["Sc"] = 1.6;
  this->calculated["Sc"] = 1.84;
  this->vanderwaals["Sc"] = 2.11;
  this->covalent_single_bond["Sc;"] = 1.48;
  this->covalent_triple_bond["Sc;"] = 1.14;
  this->emprical["Ti"] = 1.4;
  this->calculated["Ti"] = 1.76;
  this->covalent_single_bond["Ti;"] = 1.36;
  this->covalent_triple_bond["Ti;"] = 1.08;
  this->emprical["V"] = 1.35;
  this->calculated["V"] = 1.71;
  this->covalent_single_bond["V;"] = 1.34;
  this->covalent_triple_bond["V;"] = 1.06;
  this->emprical["Cr"] = 1.4;
  this->calculated["Cr"] = 1.66;
  this->covalent_single_bond["Cr;"] = 1.22;
  this->covalent_triple_bond["Cr;"] = 1.03;
  this->emprical["Mn"] = 1.4;
  this->calculated["Mn"] = 1.61;
  this->covalent_single_bond["Mn;"] = 1.19;
  this->covalent_triple_bond["Mn;"] = 1.03;
  this->emprical["Fe"] = 1.4;
  this->calculated["Fe"] = 1.56;
  this->covalent_single_bond["Fe;"] = 1.16;
  this->covalent_triple_bond["Fe;"] = 1.02;
  this->emprical["Co"] = 1.35;
  this->calculated["Co"] = 1.52;
  this->covalent_single_bond["Co;"] = 1.11;
  this->covalent_triple_bond["Co;"] = 0.96;
  this->emprical["Ni"] = 1.35;
  this->calculated["Ni"] = 1.49;
  this->vanderwaals["Ni"] = 1.63;
  this->covalent_single_bond["Ni;"] = 1.1;
  this->covalent_triple_bond["Ni;"] = 1.01;
  this->emprical["Cu"] = 1.35;
  this->calculated["Cu"] = 1.45;
  this->vanderwaals["Cu"] = 1.4;
  this->covalent_single_bond["Cu;"] = 1.12;
  this->covalent_triple_bond["Cu;"] = 1.2;
  this->emprical["Zn"] = 1.35;
  this->calculated["Zn"] = 1.42;
  this->vanderwaals["Zn"] = 1.39;
  this->covalent_single_bond["Zn;"] = 1.18;
  this->emprical["Ga"] = 1.3;
  this->calculated["Ga"] = 1.36;
  this->vanderwaals["Ga"] = 1.87;
  this->covalent_single_bond["Ga;"] = 1.24;
  this->covalent_triple_bond["Ga;"] = 1.21;
  this->emprical["Ge"] = 1.25;
  this->calculated["Ge"] = 1.25;
  this->vanderwaals["Ge"] = 2.11;
  this->covalent_single_bond["Ge;"] = 1.21;
  this->covalent_triple_bond["Ge;"] = 1.14;
  this->emprical["As"] = 1.15;
  this->calculated["As"] = 1.14;
  this->vanderwaals["As"] = 1.85;
  this->covalent_single_bond["As;"] = 1.21;
  this->covalent_triple_bond["As;"] = 1.06;
  this->emprical["Se"] = 1.15;
  this->calculated["Se"] = 1.03;
  this->vanderwaals["Se"] = 1.9;
  this->covalent_single_bond["Se;"] = 1.16;
  this->covalent_triple_bond["Se;"] = 1.07;
  this->emprical["Br"] = 1.15;
  this->calculated["Br"] = 0.94;
  this->vanderwaals["Br"] = 1.85;
  this->covalent_single_bond["Br;"] = 1.14;
  this->covalent_triple_bond["Br;"] = 1.1;
  this->calculated["Kr"] = 0.88;
  this->vanderwaals["Kr"] = 2.02;
  this->covalent_single_bond["Kr;"] = 1.17;
  this->covalent_triple_bond["Kr;"] = 1.08;
  this->emprical["Rb"] = 2.35;
  this->calculated["Rb"] = 2.65;
  this->vanderwaals["Rb"] = 3.03;
  this->covalent_single_bond["Rb;"] = 2.1;
  this->emprical["Sr"] = 2.0;
  this->calculated["Sr"] = 2.19;
  this->vanderwaals["Sr"] = 2.49;
  this->covalent_single_bond["Sr;"] = 1.85;
  this->covalent_triple_bond["Sr;"] = 1.39;
  this->emprical["Y"] = 1.8;
  this->calculated["Y"] = 2.12;
  this->covalent_single_bond["Y;"] = 1.63;
  this->covalent_triple_bond["Y;"] = 1.24;
  this->emprical["Zr"] = 1.55;
  this->calculated["Zr"] = 2.06;
  this->covalent_single_bond["Zr;"] = 1.54;
  this->covalent_triple_bond["Zr;"] = 1.21;
  this->emprical["Nb"] = 1.45;
  this->calculated["Nb"] = 1.98;
  this->covalent_single_bond["Nb;"] = 1.47;
  this->covalent_triple_bond["Nb;"] = 1.16;
  this->emprical["Mo"] = 1.45;
  this->calculated["Mo"] = 1.9;
  this->covalent_single_bond["Mo;"] = 1.38;
  this->covalent_triple_bond["Mo;"] = 1.13;
  this->emprical["Tc"] = 1.35;
  this->calculated["Tc"] = 1.83;
  this->covalent_single_bond["Tc;"] = 1.28;
  this->covalent_triple_bond["Tc;"] = 1.1;
  this->emprical["Ru"] = 1.3;
  this->calculated["Ru"] = 1.78;
  this->covalent_single_bond["Ru;"] = 1.25;
  this->covalent_triple_bond["Ru;"] = 1.03;
  this->emprical["Rh"] = 1.35;
  this->calculated["Rh"] = 1.73;
  this->covalent_single_bond["Rh;"] = 1.25;
  this->covalent_triple_bond["Rh;"] = 1.06;
  this->emprical["Pd"] = 1.4;
  this->calculated["Pd"] = 1.69;
  this->vanderwaals["Pd"] = 1.63;
  this->covalent_single_bond["Pd;"] = 1.2;
  this->covalent_triple_bond["Pd;"] = 1.12;
  this->emprical["Ag"] = 1.6;
  this->calculated["Ag"] = 1.65;
  this->vanderwaals["Ag"] = 1.72;
  this->covalent_single_bond["Ag;"] = 1.28;
  this->covalent_triple_bond["Ag;"] = 1.37;
  this->emprical["Cd"] = 1.55;
  this->calculated["Cd"] = 1.61;
  this->vanderwaals["Cd"] = 1.58;
  this->covalent_single_bond["Cd;"] = 1.36;
  this->emprical["In"] = 1.55;
  this->calculated["In"] = 1.56;
  this->vanderwaals["In"] = 1.93;
  this->covalent_single_bond["In;"] = 1.42;
  this->covalent_triple_bond["In;"] = 1.46;
  this->emprical["Sn"] = 1.45;
  this->calculated["Sn"] = 1.45;
  this->vanderwaals["Sn"] = 2.17;
  this->covalent_single_bond["Sn;"] = 1.4;
  this->covalent_triple_bond["Sn;"] = 1.32;
  this->emprical["Sb"] = 1.45;
  this->calculated["Sb"] = 1.33;
  this->vanderwaals["Sb"] = 2.06;
  this->covalent_single_bond["Sb;"] = 1.4;
  this->covalent_triple_bond["Sb;"] = 1.27;
  this->emprical["Te"] = 1.4;
  this->calculated["Te"] = 1.23;
  this->vanderwaals["Te"] = 2.06;
  this->covalent_single_bond["Te;"] = 1.36;
  this->covalent_triple_bond["Te;"] = 1.21;
  this->emprical["I"] = 1.4;
  this->calculated["I"] = 1.15;
  this->vanderwaals["I"] = 1.98;
  this->covalent_single_bond["I;"] = 1.33;
  this->covalent_triple_bond["I;"] = 1.25;
  this->calculated["Xe"] = 1.08;
  this->vanderwaals["Xe"] = 2.16;
  this->covalent_single_bond["Xe;"] = 1.31;
  this->covalent_triple_bond["Xe;"] = 1.22;
  this->emprical["Cs"] = 2.65;
  this->calculated["Cs"] = 2.98;
  this->vanderwaals["Cs"] = 3.43;
  this->covalent_single_bond["Cs;"] = 2.32;
  this->emprical["Ba"] = 2.15;
  this->calculated["Ba"] = 2.53;
  this->vanderwaals["Ba"] = 2.68;
  this->covalent_single_bond["Ba;"] = 1.96;
  this->covalent_triple_bond["Ba;"] = 1.49;
  this->emprical["La"] = 1.95;
  this->calculated["La"] = 2.26;
  this->covalent_single_bond["La;"] = 1.8;
  this->covalent_triple_bond["La;"] = 1.39;
  this->emprical["Ce"] = 1.85;
  this->calculated["Ce"] = 2.1;
  this->covalent_single_bond["Ce;"] = 1.63;
  this->covalent_triple_bond["Ce;"] = 1.31;
  this->emprical["Pr"] = 1.85;
  this->calculated["Pr"] = 2.47;
  this->covalent_single_bond["Pr;"] = 1.76;
  this->covalent_triple_bond["Pr;"] = 1.28;
  this->emprical["Nd"] = 1.85;
  this->calculated["Nd"] = 2.06;
  this->covalent_single_bond["Nd;"] = 1.74;
  this->emprical["Pm"] = 1.85;
  this->calculated["Pm"] = 2.05;
  this->covalent_single_bond["Pm;"] = 1.73;
  this->emprical["Sm"] = 1.85;
  this->calculated["Sm"] = 2.38;
  this->covalent_single_bond["Sm;"] = 1.72;
  this->emprical["Eu"] = 1.85;
  this->calculated["Eu"] = 2.31;
  this->covalent_single_bond["Eu;"] = 1.68;
  this->emprical["Gd"] = 1.8;
  this->calculated["Gd"] = 2.33;
  this->covalent_single_bond["Gd;"] = 1.69;
  this->covalent_triple_bond["Gd;"] = 1.32;
  this->emprical["Tb"] = 1.75;
  this->calculated["Tb"] = 2.25;
  this->covalent_single_bond["Tb;"] = 1.68;
  this->emprical["Dy"] = 1.75;
  this->calculated["Dy"] = 2.28;
  this->covalent_single_bond["Dy;"] = 1.67;
  this->emprical["Ho"] = 1.75;
  this->calculated["Ho"] = 2.26;
  this->covalent_single_bond["Ho;"] = 1.66;
  this->emprical["Er"] = 1.75;
  this->calculated["Er"] = 2.26;
  this->covalent_single_bond["Er;"] = 1.65;
  this->emprical["Tm"] = 1.75;
  this->calculated["Tm"] = 2.22;
  this->covalent_single_bond["Tm;"] = 1.64;
  this->emprical["Yb"] = 1.75;
  this->calculated["Yb"] = 2.22;
  this->covalent_single_bond["Yb;"] = 1.7;
  this->emprical["Lu"] = 1.75;
  this->calculated["Lu"] = 2.17;
  this->covalent_single_bond["Lu;"] = 1.62;
  this->covalent_triple_bond["Lu;"] = 1.31;
  this->emprical["Hf"] = 1.55;
  this->calculated["Hf"] = 2.08;
  this->covalent_single_bond["Hf;"] = 1.52;
  this->covalent_triple_bond["Hf;"] = 1.22;
  this->emprical["Ta"] = 1.45;
  this->calculated["Ta"] = 2.0;
  this->covalent_single_bond["Ta;"] = 1.46;
  this->covalent_triple_bond["Ta;"] = 1.19;
  this->emprical["W"] = 1.35;
  this->calculated["W"] = 1.93;
  this->covalent_single_bond["W;"] = 1.37;
  this->covalent_triple_bond["W;"] = 1.15;
  this->emprical["Re"] = 1.35;
  this->calculated["Re"] = 1.88;
  this->covalent_single_bond["Re;"] = 1.31;
  this->covalent_triple_bond["Re;"] = 1.1;
  this->emprical["Os"] = 1.3;
  this->calculated["Os"] = 1.85;
  this->covalent_single_bond["Os;"] = 1.29;
  this->covalent_triple_bond["Os;"] = 1.09;
  this->emprical["Ir"] = 1.35;
  this->calculated["Ir"] = 1.8;
  this->covalent_single_bond["Ir;"] = 1.22;
  this->covalent_triple_bond["Ir;"] = 1.07;
  this->emprical["Pt"] = 1.35;
  this->calculated["Pt"] = 1.77;
  this->vanderwaals["Pt"] = 1.75;
  this->covalent_single_bond["Pt;"] = 1.23;
  this->covalent_triple_bond["Pt;"] = 1.1;
  this->emprical["Au"] = 1.35;
  this->calculated["Au"] = 1.74;
  this->vanderwaals["Au"] = 1.66;
  this->covalent_single_bond["Au;"] = 1.24;
  this->covalent_triple_bond["Au;"] = 1.23;
  this->emprical["Hg"] = 1.5;
  this->calculated["Hg"] = 1.71;
  this->vanderwaals["Hg"] = 1.55;
  this->covalent_single_bond["Hg;"] = 1.33;
  this->emprical["Tl"] = 1.9;
  this->calculated["Tl"] = 1.56;
  this->vanderwaals["Tl"] = 1.96;
  this->covalent_single_bond["Tl;"] = 1.44;
  this->covalent_triple_bond["Tl;"] = 1.5;
  this->emprical["Pb"] = 1.8;
  this->calculated["Pb"] = 1.54;
  this->vanderwaals["Pb"] = 2.02;
  this->covalent_single_bond["Pb;"] = 1.44;
  this->covalent_triple_bond["Pb;"] = 1.37;
  this->emprical["Bi"] = 1.6;
  this->calculated["Bi"] = 1.43;
  this->vanderwaals["Bi"] = 2.07;
  this->covalent_single_bond["Bi;"] = 1.51;
  this->covalent_triple_bond["Bi;"] = 1.35;
  this->emprical["Po"] = 1.9;
  this->calculated["Po"] = 1.35;
  this->vanderwaals["Po"] = 1.97;
  this->covalent_single_bond["Po;"] = 1.45;
  this->covalent_triple_bond["Po;"] = 1.29;
  this->calculated["At"] = 1.27;
  this->vanderwaals["At"] = 2.02;
  this->covalent_single_bond["At;"] = 1.47;
  this->covalent_triple_bond["At;"] = 1.38;
  this->calculated["Rn"] = 1.2;
  this->vanderwaals["Rn"] = 2.2;
  this->covalent_single_bond["Rn;"] = 1.42;
  this->covalent_triple_bond["Rn;"] = 1.33;
  this->vanderwaals["Fr"] = 3.48;
  this->emprical["Ra"] = 2.15;
  this->vanderwaals["Ra"] = 2.83;
  this->covalent_single_bond["Ra;"] = 2.01;
  this->covalent_triple_bond["Ra;"] = 1.59;
  this->emprical["Ac"] = 1.95;
  this->covalent_single_bond["Ac;"] = 1.86;
  this->covalent_triple_bond["Ac;"] = 1.4;
  this->emprical["Th"] = 1.8;
  this->covalent_single_bond["Th;"] = 1.75;
  this->covalent_triple_bond["Th;"] = 1.36;
  this->emprical["Pa"] = 1.8;
  this->covalent_single_bond["Pa;"] = 1.69;
  this->covalent_triple_bond["Pa;"] = 1.29;
  this->emprical["U"] = 1.75;
  this->vanderwaals["U"] = 1.86;
  this->covalent_single_bond["U;"] = 1.7;
  this->covalent_triple_bond["U;"] = 1.18;
  this->emprical["Np"] = 1.75;
  this->covalent_single_bond["Np;"] = 1.71;
  this->covalent_triple_bond["Np;"] = 1.16;
  this->emprical["Pu"] = 1.75;
  this->covalent_single_bond["Pu;"] = 1.72;
  this->emprical["Am"] = 1.75;
  this->covalent_single_bond["Am;"] = 1.66;
  this->emprical["Cm"] = 1.76;
  this->covalent_single_bond["Cm;"] = 1.66;
  this->covalent_triple_bond["Rf;"] = 1.31;
  this->covalent_triple_bond["Db;"] = 1.26;
  this->covalent_triple_bond["Sg;"] = 1.21;
  this->covalent_triple_bond["Bh;"] = 1.19;
  this->covalent_triple_bond["Hs;"] = 1.18;
  this->covalent_triple_bond["Mt;"] = 1.13;
  this->covalent_triple_bond["Ds;"] = 1.12;
  this->covalent_triple_bond["Rg;"] = 1.18;
  this->covalent_triple_bond["Cn;"] = 1.3;
}


} // end namespace atomsciflow

#endif // ATOMSCIFLOW_BASE_ATOMIC_RADIUS_H_
