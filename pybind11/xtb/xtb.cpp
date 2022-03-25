/// @file pybind11/xtb/xtb.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 04:14:16 PM CST

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/xtb/xtb.h"

namespace py = pybind11;

void add_class_xtb(py::module& m) {
    py::class_<atomsciflow::Xtb>(m, "Xtb")
        .def(py::init<>())
        ;
}

PYBIND11_MODULE(xtb, m) {
    m.doc() = "xtb module";
    
    add_class_xtb(m);
}