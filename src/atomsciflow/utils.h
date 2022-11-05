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

// The documents generated by doxygen are regarded as
// derivative works from the input, namely the source
// files to extract comments. Thus they are not affected
// by the license doxygen used.
// This indication is clearly outlined in doxygen's license
// disclaimer. So, it's OK to use doxygen styled comments
// in atomsciflow project.

/// @file src/atomsciflow/utils.h
/// @author DeqiTang
/// Mail: deqitang@gmail.com

#ifndef ATOMSCIFLOW_UTILS_H_
#define ATOMSCIFLOW_UTILS_H_

#include <string>

#include "atomsciflow/base/crystal.h"
#include "atomsciflow/base/atom.h"

namespace atomsciflow {

std::string version();

int move_along(atomsciflow::Crystal* structure, std::vector<int> atoms_to_move, std::vector<int> direc, double disp);

int remove_atoms(atomsciflow::Crystal* structure, std::vector<int> atoms_to_remove);

int vacuum_layer(atomsciflow::Crystal* structure, std::string plane, double thickness);

int set_frac_min_to_zero(atomsciflow::Crystal* structure);

int set_frac_within_zero_and_one(atomsciflow::Crystal* structure);

int inverse_geo_center(atomsciflow::Crystal* structure);

int inverse_point(atomsciflow::Crystal* structure, std::vector<double> point);

int inverse_cell_center(atomsciflow::Crystal* structure);

int rotate_along_axis(atomsciflow::Crystal* structure, std::vector<int> rotate_atoms, std::vector<int> axis);

std::vector<atomsciflow::Atom> enlarge_atoms(atomsciflow::Crystal* structure);

std::vector<atomsciflow::Atom> enlarge_atoms_new_cell(atomsciflow::Crystal* structure, std::vector<std::vector<double> > new_cell);

int redefine_lattice(atomsciflow::Crystal* structure, std::vector<int> a, std::vector<int> b, std::vector<int> c, double precision);

int cleave_surface(atomsciflow::Crystal* structure, std::vector<int> direction, double thickness, double precision);

atomsciflow::Crystal merge_layers(atomsciflow::Crystal* structure1, atomsciflow::Crystal* structure2, int use_cell, double distance, double thickness);

atomsciflow::Crystal build_nanotube_ab(atomsciflow::Crystal* structure, std::string axis);

atomsciflow::Crystal build_nanotube_ac(atomsciflow::Crystal* structure, std::string axis);

atomsciflow::Crystal build_nanotube_bc(atomsciflow::Crystal* structure, std::string axis);

} //namespace atomsciflow

#endif // ATOMSCIFLOW_UTILS_H_
