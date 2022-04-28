#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/plumed/plumed.h"

namespace py = pybind11;

void add_class_plumed(py::module& m) {
    py::class_<atomsciflow::plumed::Plumed>(m, "Plumed")
        .def(py::init<>())
        .def("to_string", &atomsciflow::plumed::Plumed::to_string)
        .def("add_action", &atomsciflow::plumed::Plumed::add_action)
        .def_readwrite("actions", &atomsciflow::plumed::Plumed::actions)
        ;
}

void add_class_plumed_action(py::module& m) {
    py::class_<atomsciflow::plumed::Action>(m, "Action")
    .def(py::init<>())
    .def("to_string", &atomsciflow::plumed::Action::to_string)
    .def("add_keyword", &atomsciflow::plumed::Action::add_keyword)
    .def_readwrite("label", &atomsciflow::plumed::Action::label)
    .def_readwrite("name", &atomsciflow::plumed::Action::name)
    .def_readwrite("keywords", &atomsciflow::plumed::Action::keywords)
    ;
}

PYBIND11_MODULE(plumed, m) {
    m.doc() = "plumed module";
    
    add_class_plumed(m);
    add_class_plumed_action(m);
}
