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

/// @file src/atomsciflow/qe/gen_namelist.cpp
/// @author deqi
/// Mail: deqi_tang@163.com
/// Created Time: Tue 02 Feb 2021 05:39:55 PM CST

#include "atomsciflow/qe/gen_namelist.h"

namespace atomsciflow::qe::gen {

Namelist control() {
    Namelist out;    
    out.name = "control";
    out.set_param("calculation", "scf");
    out.set_param("verbosity", "low");
    return out;
}

Namelist system() {
    Namelist out;
    out.name = "system";
    out.set_param("input_dft", "PBE");
    return out;
}

Namelist electrons() {
    Namelist out;
    out.name = "electrons";
    out.set_param("electron_maxstep", 100);
    out.set_param("conv_thr", "1.0E-6");
    return out;
}

Namelist ions() {
    Namelist out;
    out.name = "ions";
    out.set_param("ion_dynamics", "bfgs");
    return out;
}

Namelist cell() {
    Namelist out;
    out.name = "cell";
    out.set_param("cell_dynamics", "bfgs");
    return out;
}

} // atomsciflow::qe::gen
