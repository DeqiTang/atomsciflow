/// @file pybind11/octopus/octopus.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 04:13:57 PM CST

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/octopus/octopus.h"

namespace py = pybind11;

void add_class_octopus(py::module& m) {
    py::class_<atomsciflow::Octopus>(m, "Octopus")
        .def(py::init<>())
        ;
}

PYBIND11_MODULE(octopus, m) {
    m.doc() = "octopus module";
    
    add_class_octopus(m);
}