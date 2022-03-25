/// @file pybind11/gaussian/gaussian.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 04:13:33 PM CST

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/gaussian/gaussian.h"

namespace py = pybind11;

void add_class_gaussian(py::module& m) {
    py::class_<atomsciflow::Gaussian>(m, "Gaussian")
        .def(py::init<>())
        ;
}

PYBIND11_MODULE(gaussian, m) {
    m.doc() = "gaussian module";
    
    add_class_gaussian(m);
}