#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/plumed/plumed.h"

namespace py = pybind11;

void add_class_plumed_action(py::module& m) {
    py::class_<atomsciflow::plumed::Action>(m, "Action")
    .def(py::init<>())
    .def("to_string", &atomsciflow::plumed::Action::to_string)
    .def_readwrite("label", &atomsciflow::plumed::Action::label)
    .def_readwrite("name", &atomsciflow::plumed::Action::name)
    ;
}

void add_class_plumed(py::module& m) {
    py::class_<atomsciflow::plumed::Plumed>(m, "Plumed")
        .def(py::init<>())
        .def("to_string", &atomsciflow::plumed::Plumed::to_string)
        .def("add_action", &atomsciflow::plumed::Plumed::add_action)
        .def("set_param", py::overload_cast<const std::string&, int>(&atomsciflow::plumed::Plumed::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, double>(&atomsciflow::plumed::Plumed::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string>(&atomsciflow::plumed::Plumed::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<int>>(&atomsciflow::plumed::Plumed::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<double>>(&atomsciflow::plumed::Plumed::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::string>>(&atomsciflow::plumed::Plumed::py_set_param))
        .def_readwrite("actions", &atomsciflow::plumed::Plumed::actions)
        ;
}


PYBIND11_MODULE(plumed, m) {
    m.doc() = "plumed module";
    
    add_class_plumed(m);
    add_class_plumed_action(m);
}
