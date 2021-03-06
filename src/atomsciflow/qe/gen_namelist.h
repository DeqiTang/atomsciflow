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

/// @file src/atomsciflow/qe/gen_namelist.h
/// @author deqi
/// Mail: deqi_tang@163.com
/// Created Time: Tue 02 Feb 2021 05:30:54 PM CST

#ifndef ATOMSCIFLOW_QE_GEN_NAMELIST_H_
#define ATOMSCIFLOW_QE_GEN_NAMELIST_H_

#include "atomsciflow/qe/namelist.h"

namespace atomsciflow {

namespace qe {
namespace gen {      

Namelist control();

Namelist system();

Namelist electrons();

Namelist ions();

Namelist cell();

} // namespace gen
} // namespace qe
} // namespace atomsciflow

#endif // ATOMSCIFLOW_QE_GEN_NAMELIST_H_
