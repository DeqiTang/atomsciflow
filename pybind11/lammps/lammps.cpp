/// @file pybind11/lammps/lammps.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 04:13:46 PM CST

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/lammps/lammps.h"

namespace py = pybind11;

void add_class_lammps(py::module& m) {
    py::class_<atomsciflow::Lammps>(m, "Lammps")
        .def(py::init<>())
        ;
}

PYBIND11_MODULE(lammps, m) {
    m.doc() = "lammps module";
    
    add_class_lammps(m);
}