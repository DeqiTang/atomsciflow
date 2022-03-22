
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/qe/pwscf.h"

namespace py = pybind11;

void add_class_pwscf(py::module& m) {
    
    py::class_<atomsciflow::qe::PwScf>(m, "PwScf")
        .def(py::init<>())
        .def("get_xyz", &atomsciflow::qe::PwScf::get_xyz)
        .def("to_string", py::overload_cast<>(&atomsciflow::qe::PwScf::to_string))
        .def("to_string", py::overload_cast<std::string>(&atomsciflow::qe::PwScf::to_string))
        .def("run", &atomsciflow::qe::PwScf::run)
        ;
}

PYBIND11_MODULE(qe, m) {
    m.doc() = "qe module";
    m.attr("__version__") = "0.0.1";

    add_class_pwscf(m);
}

