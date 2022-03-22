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

/// @file src/atomsciflow/qe/misc.h
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Tue 08 Mar 2022 09:25:52 PM CST

#ifndef ATOMSCIFLOW_QE_MISC_H_
#define ATOMSCIFLOW_QE_MISC_H_

#include <map>
#include <set>

#include "atomsciflow/base/xyz.h"
#include "atomsciflow/variable/group.h"
#include "atomsciflow/base/kpath.h"

namespace atomsciflow {

class PwScfPseudo {
public:
    PwScfPseudo();

    std::string to_string(Xyz& xyz);

    std::string directory;
};

/**
 * @brief The PwScfMisc class
 * Responsible for the manimulation and controlling of miscellaneous
 * input blocks in PwScf, including ATOMIC_SPECIES, ATOMIC_POSITIONS,
 * KPOINTS, CELL_PARAMETERS, CONSTRAINTS, OCCUPATIONS, ATOMIC_FORCES
 */
class PwScfMisc : public VariableGroup {
public:

    PwScfMisc();

    std::string to_string(std::string coordtype);
    void set_kpoints(std::string option, std::vector<int>& kpoints_mp, Kpath crystal_b);
    std::string to_string_kpoints();
    std::string to_string_atomic_forces();

    void set_atomic_forces(std::string pressure, std::string direction);

    void basic_setting(bool ifstatic);

    Xyz xyz;
    std::map<std::string, std::string> cell_params;
    PwScfPseudo pseudo;
    std::string kpoints_option;
    std::vector<int> kpoints_mp;
    /// @var ifstatic bool
    ///  determine whether to set the fixing state
    ///  on atoms
    bool ifstatic;
    /// @var atomic_forces_status bool
    ///  set to false by default, namely no forces
    ///  acting on atoms
    bool atomic_forces_status;
    std::vector<std::vector<double>> atomic_forces;
    Kpath crystal_b;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_QE_MISC_H_
