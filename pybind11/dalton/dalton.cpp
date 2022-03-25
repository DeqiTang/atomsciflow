/// @file pybind11/dalton/dalton.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Thu 24 Mar 2022 09:25:03 PM CST

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/dalton/dalton.h"

namespace py = pybind11;

void add_class_dalton(py::module& m) {
    py::class_<atomsciflow::Dalton>(m, "Dalton")
        .def(py::init<>())
        .def("to_string", &atomsciflow::Dalton::to_string)
        .def("get_xyz", &atomsciflow::Dalton::get_xyz)
        .def("run", &atomsciflow::Dalton::run)
        .def_readwrite("job", &atomsciflow::Dalton::job)
        ;
}

PYBIND11_MODULE(dalton, m) {
    m.doc() = "dalton module";
    m.attr("__version__") = "0.0.0";

    add_class_dalton(m);
}
