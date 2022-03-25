/// @file pybind11/siesta/siesta.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Thu 24 Mar 2022 09:35:01 PM CST

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/siesta/siesta.h"
#include "atomsciflow/siesta/opt.h"

namespace py = pybind11;

void add_class_siesta(py::module& m) {
    py::class_<atomsciflow::Siesta>(m, "Siesta")
        .def(py::init<>())
        .def("to_string", &atomsciflow::Siesta::to_string)
        .def("get_xyz", &atomsciflow::Siesta::get_xyz)
        .def("run", &atomsciflow::Siesta::run)
        .def_readwrite("job", &atomsciflow::Siesta::job)
        ;
}

void add_class_siestaopt(py::module& m) {
    py::class_<atomsciflow::SiestaOpt>(m, "SiestaOpt")
        .def(py::init<>())
        .def("to_string", &atomsciflow::SiestaOpt::to_string)
        .def("get_xyz", &atomsciflow::SiestaOpt::get_xyz)
        .def("run", &atomsciflow::SiestaOpt::run)
        .def_readwrite("job", &atomsciflow::SiestaOpt::job)
        ;
}

PYBIND11_MODULE(siesta, m) {
    m.doc() = "siesta module";
    m.attr("__version__") = "0.0.0";

    add_class_siesta(m);
    add_class_siestaopt(m);
}
