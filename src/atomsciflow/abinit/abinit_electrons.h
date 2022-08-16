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

/// @file src/atomsciflow/abinit/abinit_electrons.h
/// @author Deqi Tang
/// Mail: deqi_tang@163.com
/// Created Time: Mon 25 Jan 2021 07:24:49 PM CST

#ifndef ATOMSCIFLOW_ABINIT_ABINIT_ELECTRONS_H_
#define ATOMSCIFLOW_ABINIT_ABINIT_ELECTRONS_H_

#include <string>
#include <map>
#include <set>

#include "atomsciflow/base/kpath.h"
#include "atomsciflow/variable/group.h"

namespace atomsciflow {

/**
 * @brief The Kpoints class
 * Taking over the setting of Kpoints in Abinit calculation
 * THe most simple configuration for kpoints might be
 * kptopt = 1, plus ngkpt = {1, 1, 1}
 *
 * Reference:
 * https://docs.abinit.org/variables/basic/#kptopt
 * https://docs.abinit.org/variables/gstate/#kptbounds
 */
class Kpoints : public VariableGroup {
public:
    Kpoints() {
        this->incharge.insert("kptopt");
        this->incharge.insert("ngkpt");
        this->incharge.insert("nshiftk");
        this->incharge.insert("shiftk");
        this->basic_setting();
    }

    ~Kpoints() {
    }

    void basic_setting() {
        this->set_param("kptopt", 1);
        this->set_param("ngkpt", std::vector<int>{1, 1, 1});
    }

    void set_band(Kpath kpath) {
        this->kpath = kpath;
        this->set_param("kptopt", -(this->kpath.nkpoint - 1));
    }

    std::set<std::string> incharge;
    Kpath kpath;   
};


class AbinitElectrons : public VariableGroup {
public:
    AbinitElectrons() {
        this->incharge.insert("ecut");
        this->incharge.insert("ixc");
        this->incharge.insert("nstep");
        this->incharge.insert("diemac");
        this->incharge.insert("iscf");
        this->incharge.insert("toldfe");
        this->incharge.insert("tolwfr");
        this->incharge.insert("toldff");
        this->incharge.insert("tolrff");
        this->incharge.insert("tolvrs");
        this->incharge.insert("occopt");
        this->incharge.insert("nband");
        this->incharge.insert("occ");
        this->incharge.insert("wtk");
        this->incharge.insert("prtden");
        this->incharge.insert("prtdos");

        this->status = true;

        // this->basic_setting();
    }

    ~AbinitElectrons() {
    }

    std::string to_string(int n);
    void use_tol(std::string tol, double value);
    void basic_setting();
    void set_scf_nscf(std::string mode);

    std::set<std::string> incharge;
    bool status;
    Kpoints kpoints;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_ABINIT_ABINIT_ELECTRONS_H_
