/// @file pybind11/orca/orca.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 04:14:03 PM CST

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/orca/orca.h"

namespace py = pybind11;

void add_class_orca(py::module& m) {
    py::class_<atomsciflow::Orca>(m, "Orca")
        .def(py::init<>())
        ;
}

PYBIND11_MODULE(orca, m) {
    m.doc() = "orca module";
    
    add_class_orca(m);
}