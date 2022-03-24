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

/// module for xyz structure

#ifndef ATOMSCIFLOW_BASE_XYZ_H_
#define ATOMSCIFLOW_BASE_XYZ_H_

#include <map>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include "atomsciflow/base/atom.h"

namespace atomsciflow {

class Xyz {
public:

    Xyz() {};
    ~Xyz() {};
    
    int read_xyz_file(std::string filepath);
    int write_xyz_file(std::string filepath);

    std::vector<std::vector<double>> get_cell() {
        return this->cell;
    }

    int get_atoms(Atom* atoms) {
        return 0;
    }

    int get_cell_atoms(double** cell, Atom* atoms) {
        return 0;
    }

    int cartesian() {
        return 0;
    }

    int get_fractional() {
        return 0;
    }

    double volume() {
        return 0;
    }

    int natom() {
        return this->atoms.size();
    }
    
    std::string file;
    int nspecies;
    std::set<std::string> elements_set;
    std::vector<Atom> atoms;
    std::vector<std::vector<double>> cell;

private:

};

} //namespace atomsciflow

#endif // ATOMSCIFLOW_BASE_XYZ_H_
