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

/// @file src/atomsciflow/utils/structure.h
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Wed 09 Mar 2022 08:01:07 PM CST

#ifndef ATOMSCIFLOW_UTILS_STRUCTURE_H_
#define ATOMSCIFLOW_UTILS_STRUCTURE_H_

#include <vector>

#include "atomsciflow/base/atom.h"

namespace atomsciflow {

void atoms_cart_to_frac(std::vector<Atom>& atoms_frac, const std::vector<Atom>& atoms_cart, const std::vector<std::vector<double>>& cell);
void atoms_frac_to_cart(std::vector<Atom>& atoms_cart, const std::vector<Atom>& atoms_frac, const std::vector<std::vector<double>>& cell);

} // namespace atomsciflow

#endif // ATOMSCIFLOW_UTILS_STRUCTURE_H_
