// @file pybind11/abinit/abinit.cpp
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Thu 06 Jan 2022 07:50:41 PM CST

#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL

#include "atomsciflow/abinit/abinit.h"

namespace py = pybind11;

void add_class_abinit(py::module& m) {

    py::class_<atomsciflow::Abinit>(m, "Abinit")
        .def(py::init<>())
        .def("get_xyz", &atomsciflow::Abinit::get_xyz)
        .def("to_string", &atomsciflow::Abinit::to_string)
        .def("run", &atomsciflow::Abinit::run)
        .def_readwrite("job", &atomsciflow::Abinit::job)
        ;
}

PYBIND11_MODULE(abinit, m) {
    m.doc() = "abinit module";
    
    add_class_abinit(m);
}

