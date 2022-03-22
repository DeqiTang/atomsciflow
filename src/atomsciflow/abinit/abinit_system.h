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

/// @file src/atomsciflow/abinit/abinit_system.h
/// @author Deqi Tang
/// Mail: deqi_tang@163.com
/// Created Time: Sun 24 Jan 2021 07:59:44 PM CST

#ifndef ATOMSCIFLOW_ABINIT_ABINIT_SYSTEM_H_
#define ATOMSCIFLOW_ABINIT_ABINIT_SYSTEM_H_

#include <string>
#include <map>
#include <armadillo>
#include <boost/format.hpp>

#include "atomsciflow/base/xyz.h"
#include "atomsciflow/base/element.h"
#include "atomsciflow/utils/structure.h"

namespace atomsciflow {

/**
 * @brief The AbinitSystem class
 * Note:
 * xcart is recommended for Abinit v9.2 and later,
 * while the previous xangst varialbe is deprecated.
 */
class AbinitSystem {
public:
    AbinitSystem() {
        this->status = true;
        this->coordtype = "cartesian";
    }
    void get_xyz(const std::string& xyzfile);

    std::string to_string(int n);

    //
    Xyz xyz;
    bool status;
    std::map<std::string, int> elem_typat;
    /// \brief coordtype
    /// candidates:
    ///     "cartesian" | "reduced"
    std::string coordtype;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_ABINIT_ABINIT_SYSTEM_H_
