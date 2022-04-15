#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/plumed/plumed.h"

namespace py = pybind11;

void add_class_plumed(py::module& m) {
    py::class_<atomsciflow::plumed::Plumed>(m, "Plumed")
        .def(py::init<>())
        ;
}

PYBIND11_MODULE(plumed, m) {
    m.doc() = "plumed module";
    
    add_class_plumed(m);
}
