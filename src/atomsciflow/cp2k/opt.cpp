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

/// @file src/atomsciflow/cp2k/opt.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Sun 23 Jan 2022 03:20:37 PM CST

#include "atomsciflow/cp2k/opt.h"

//#include <filesystem>
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>

namespace atomsciflow {

//namespace fs = std::filesystem;
namespace fs = boost::filesystem;    // --std=c++11 -lboost_filesystem -lboost_system

Cp2kOpt::Cp2kOpt() {
    //this->sections["force_eval"].sections["dft"].sections["mgrid"].set_param("cutoff", 100);
    //this->sections["force_eval"].sections["dft"].sections["mgrid"].set_param("rel_cutoff", 60);
    set_param("force_eval/dft/mgrid/cutoff", 120);
    set_param("force_eval/dft/mgrid/rel_cutoff", 60);

    //this->new_section("motion");
    std::cout << "Cp2kOpt::Cp2kOpt -> new_section: motion/print\n";
    this->new_section("motion/print");
    std::cout << "Cp2kOpt::Cp2kOpt -> new_section: motion/print/forces\n";
    this->new_section("motion/print/forces");

    this->set_geo_opt();
}

void Cp2kOpt::set_geo_opt() {
    //this->sections["global"].set_param("run_type", "GEO_OPT");
    this->set_param("global/run_type", "GEO_OPT");
    this->sections["motion"]->set_status("geo_opt", true);
}

} // namespace atomsciflow
