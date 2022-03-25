/// @file pybind11/gromacs/gromacs.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 04:13:40 PM CST

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/gromacs/gromacs.h"

namespace py = pybind11;

void add_class_gromacs(py::module& m) {
    py::class_<atomsciflow::Gromacs>(m, "Gromacs")
        .def(py::init<>())
        ;
}

PYBIND11_MODULE(gromacs, m) {
    m.doc() = "gromacs module";
    
    add_class_gromacs(m);
}
