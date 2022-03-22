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

#ifndef ATOMSCIFLOW_VASP_VASP_POSCAR_H
#define ATOMSCIFLOW_VASP_VASP_POSCAR_H

#include <string>
#include <map>

#include "atomsciflow/base/xyz.h"

namespace atomsciflow {

class VaspPoscar {
public:
    VaspPoscar();
    std::string to_string(std::string coordtype);

    void get_xyz(std::string xyzfile);

    Xyz xyz;
    std::vector<std::pair<std::string, int>> elem_natom_in_number_order;
    std::map<std::string, int> specie_labels;

    bool selective_dynamics;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_VASP_VASP_POSCAR_H
