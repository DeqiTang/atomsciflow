
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/cp2k/cp2k.h"

namespace py = pybind11;


PYBIND11_MODULE(cp2k, m) {
    m.doc() = "cpptest module";
    m.attr("__version__") = "0.0.1";



    py::class_<atomsciflow::Cp2k>(m, "Cp2k")
        .def(py::init<>())
        .def("to_string", &atomsciflow::Cp2k::to_string)
        // atomsciflow::Cp2k::set_subsys is a overloaded function
        // and atomsciflow::Cp2k::set_subsys() is private
        // we don't expose it to python. so only provide the following
        .def("set_subsys", py::overload_cast<atomsciflow::Crystal&>(&atomsciflow::Cp2k::set_subsys), py::return_value_policy::reference)
        ;
}
