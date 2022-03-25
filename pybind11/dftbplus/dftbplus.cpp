/// @file pybind11/dftbplus/dftbplus.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 04:13:11 PM CST

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/dftbplus/dftbplus.h"

namespace py = pybind11;

void add_class_dftbplus(py::module& m) {
    py::class_<atomsciflow::DftbPlus>(m, "DftbPlus")
        .def(py::init<>())
        ;
}

PYBIND11_MODULE(dftbplus, m) {
    m.doc() = "dftbplus module";
    
    add_class_dftbplus(m);
}