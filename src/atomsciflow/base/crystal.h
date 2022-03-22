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

/// module for crystal structure manipulation

#ifndef ATOMSCIFLOW_BASE_CRYSTAL_H_
#define ATOMSCIFLOW_BASE_CRYSTAL_H_

#include <vector>
#include <string>
#include <algorithm>
#include "atomsciflow/base/atom.h"

namespace atomsciflow {

class Crystal {
public:

    Crystal() {};
    ~Crystal() {};
    
    int read_xyz_file(std::string filepath);
    int write_xyz_file(std::string filepath);
    int read_xyz_str(std::string str);
    int write_xyz_str(std::string& str);
    std::string write_xyz_str();

    /// @params cell; {{a1, a2, a3}, {b1, b2, b3}, {c1, c2, c3}} in unit of Anstrom
    std::vector<std::vector<double>> get_cell() {
        return this->cell;
    };

    int get_atoms(Atom* atoms) {
        return 0;
    }
    int get_cell_atoms(double** cell, Atom* atoms) {
        return 0;
    }
    int get_cell_atoms(std::vector<std::vector<double>>& cell, Atom* atoms) {
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
    int build_supercell(std::vector<int> n);
    int to_base_xyz(std::string filepath) {
        return 0;
    }
    int remove_atom(int number) {
        return 0;
    }
    int remove_atoms(std::vector<int> numbers) {
      std::vector<Atom> origin = this->atoms;
      this->atoms.clear();
      for (int i = 0; i < origin.size(); i++) {
        //atoms_to_remove.push_back(this->atoms[i]);
        if (std::count(numbers.begin(), numbers.end(), i) == 0) {
          this->atoms.push_back(origin[i]);
        }
      }
      return 0;
    }
    int natom() {
        return this->atoms.size();
    }
    std::vector<Atom> atoms;
    std::vector<std::vector<double>> cell;    
private:
};

} //namespace atomsciflow

#endif // ATOMSCIFLOW_BASE_CRYSTAL_H_
