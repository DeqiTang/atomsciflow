/// @file pybind11/exciting/exciting.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 04:13:23 PM CST

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/exciting/exciting.h"

namespace py = pybind11;

void add_class_exciting(py::module& m) {
    py::class_<atomsciflow::Exciting>(m, "Exciting")
        .def(py::init<>())
        .def("get_xyz", &atomsciflow::Exciting::get_xyz)
        .def("to_string", &atomsciflow::Exciting::to_string)
        .def("run", &atomsciflow::Exciting::run)
        .def_readwrite("input", &atomsciflow::Exciting::input)
        ;
}

PYBIND11_MODULE(exciting, m) {
    m.doc() = "exciting module";
    
    add_class_exciting(m);
}