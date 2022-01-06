
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/qe/pw.h"
#include "atomsciflow/qe/qe.h"

namespace py = pybind11;

void add_class_pw(py::module& m) {
    
    py::class_<atomsciflow::QePw>(m, "QEPw")
        .def(py::init<>())
        .def("to_string", py::overload_cast<>(&atomsciflow::QePw::to_string))
        .def("to_string", py::overload_cast<std::string>(&atomsciflow::QePw::to_string))
        ;

}

PYBIND11_MODULE(qe, m) {
    m.doc() = "qe module";
    m.attr("__version__") = "0.0.1";

    add_class_pw(m);

}
