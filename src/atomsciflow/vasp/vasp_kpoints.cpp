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

#include "atomsciflow/vasp/vasp_kpoints.h"

namespace atomsciflow {

VaspKpoints::VaspKpoints() {
    this->kpoints_mp = std::vector<int>{1, 1, 1, 0, 0, 0};
    this->line_mode = false;
}

std::string VaspKpoints::to_string() {
    std::string out = "";
    if ("automatic" == this->kpoints_option) {
        out += "K-POINTS automatic\n";
        out += "0\n";
        out += "Gamma\n";
        out += std::to_string(this->kpoints_mp[0]);
        out += " ";
        out += std::to_string(this->kpoints_mp[1]);
        out += " ";
        out += std::to_string(this->kpoints_mp[2]);
        out += "\n";
        out += std::to_string(this->kpoints_mp[3]);
        out += " ";
        out += std::to_string(this->kpoints_mp[4]);
        out += " ";
        out += std::to_string(this->kpoints_mp[5]);
        out += "\n";
    } else if ("bands" == this->kpoints_option) {
        if (true == this->line_mode) {
            out += "K-POINTS for band structure in line mode\n";
            out += std::to_string(this->kpath.links[0]); // default using the first connnect number for all the lines in line-mode
            out += "\n";
            out += "Line-mode\n";
            out += "rec\n"; // in reciprocal coordinates
            for (int i = 0; i < this->kpath.coords.size() - 1; i++) {
                if (0 != this->kpath.links[i]) {
                    out += " ";
                    out += std::to_string(this->kpath.coords[i][0]);
                    out += " ";
                    out += std::to_string(this->kpath.coords[i][1]);
                    out += " ";
                    out += std::to_string(this->kpath.coords[i][2]);
                    out += " !";
                    out += this->kpath.labels[i];
                    out += "\n";
                    out += " ";
                    out += std::to_string(this->kpath.coords[i+1][0]);
                    out += " ";
                    out += std::to_string(this->kpath.coords[i+1][0]);
                    out += " ";
                    out += std::to_string(this->kpath.coords[i+1][0]);
                    out += " !";
                    out += this->kpath.labels[i+1];
                    out += "\n";
                }
            }
        } else {
            out += "K-POINTS for band structure in explict specification (not line-mode)\n";
            int nk = 0;
            for (int i = 0; i < this->kpath.coords.size()-1; i++ ) { // the last k point should not be count
                if (this->kpath.links[i] != 0) {
                    nk += this->kpath.links[i];
                }
            }
            out += std::to_string(nk);
            out += "\n";
            out += "Reciprocal lattice\n";
            for (int i = 0; i < this->kpath.coords.size()-1; i++) {
                if (this->kpath.links[i] != 0) {
                    out += std::to_string(this->kpath.coords[i][0]);
                    out += " ";
                    out += std::to_string(this->kpath.coords[i][1]);
                    out += " ";
                    out += std::to_string(this->kpath.coords[i][2]);
                    out += " 1.0 !";
                    out += this->kpath.labels[i];
                    out += "\n";
                    for (int j = 0; j < this->kpath.links[i]-2; j++) {
                        double x = (this->kpath.coords[i+1][0] - this->kpath.coords[i][0]) / (this->kpath.links[i]-1) * (j+1) + this->kpath.coords[i][0];
                        double y = (this->kpath.coords[i+1][1] - this->kpath.coords[i][1]) / (this->kpath.links[i]-1) * (j+1) + this->kpath.coords[i][1];
                        double z = (this->kpath.coords[i+1][2] - this->kpath.coords[i][2]) / (this->kpath.links[i]-1) * (j+1) + this->kpath.coords[i][2];
                        out += std::to_string(x);
                        out += " ";
                        out += std::to_string(y);
                        out += " ";
                        out += std::to_string(z);
                        out += " 1.0\n";
                    }
                    out += std::to_string(this->kpath.coords[i+1][0]);
                    out += " ";
                    out += std::to_string(this->kpath.coords[i+1][1]);
                    out += " ";
                    out += std::to_string(this->kpath.coords[i+1][2]);
                    out += " 1.0 !";
                    out += this->kpath.labels[i+1];
                    out += "\n";
                } else {
                    continue;
                }
            }
        }
    } else {
        //
    }
    return out;
}

void VaspKpoints::set_kpoints(std::vector<int> kpoints_mp = std::vector<int>{1, 1, 1, 0, 0, 0}, std::string option="automatic", Kpath* kpath = nullptr) {
    this->kpoints_option = option;
    this->kpoints_mp = kpoints_mp;
    this->kpath = *kpath;
}

} // namespace atomsciflow
