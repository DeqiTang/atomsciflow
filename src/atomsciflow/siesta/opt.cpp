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

/// @file src/atomsciflow/siesta/opt.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 12:12:06 PM CST

#include "atomsciflow/siesta/opt.h"

namespace atomsciflow {

SiestaOpt::SiestaOpt() {
    set_param("MD.TypeOfRun", "CG");
    set_param("MD.VariableCell", "false");
    set_param("MD.ConstantVolume", "true");
    set_param("MD.MaxForceTol", "0.001 eV/Ang");
    set_param("MD.MaxStressTol", "0.01 GPa");
    set_param("MD.Steps", 60);
    set_param("MD.MaxDispl", "0.2 Bohr");
    set_param("MD.PreconditionVariableCell", "5 Ang");

    job.set_run("cmd", "$ASF_CMD_SIESTA");
    job.set_run("script_name_head", "siesta-run");
}

} // namespace atomsciflow