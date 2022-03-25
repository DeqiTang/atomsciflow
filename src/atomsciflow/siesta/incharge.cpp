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

/// @file src/atomsciflow/siesta/incharge.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 08:19:10 PM CST

#include "atomsciflow/siesta/incharge.h"

namespace atomsciflow {

SiestaInCharge::SiestaInCharge() {
    this->init();
}

void SiestaInCharge::init() {
    electrons_incharge.insert("XC.functional");
    electrons_incharge.insert("DM.Tolerance");
    electrons_incharge.insert("DM.MixingWeight");
    electrons_incharge.insert("DM.NumberPulay");
    electrons_incharge.insert("DM.AllowExtrapolation");
    electrons_incharge.insert("DM.UseSaveDM");
    electrons_incharge.insert("SolutionMethod");
    electrons_incharge.insert("MeshCutOff");

    ions_incharge.insert("MD.TypeOfRun");
    ions_incharge.insert("MD.VariableCell");
    ions_incharge.insert("MD.ConstantVolume");
    ions_incharge.insert("MD.MaxForceTol");
    ions_incharge.insert("MD.MaxStressTol");
    ions_incharge.insert("MD.Steps");
    ions_incharge.insert("MD.MaxDispl");
    ions_incharge.insert("MD.PreconditionVariableCell");

}

} // namespace atomsciflow