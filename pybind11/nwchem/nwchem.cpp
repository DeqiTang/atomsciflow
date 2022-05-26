/// @file pybind11/nwchem/nwchem.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 04:13:51 PM CST

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/nwchem/nwchem.h"

namespace py = pybind11;

void add_class_nwchem(py::module& m) {
    py::class_<atomsciflow::NWChem>(m, "NWChem")
        .def(py::init<>())        
        .def("to_string", &atomsciflow::NWChem::to_string)
        .def("add_keyword", py::overload_cast<const std::string&, int>(&atomsciflow::NWChem::py_add_keyword))
        .def("add_keyword", py::overload_cast<const std::string&, double>(&atomsciflow::NWChem::py_add_keyword))
        .def("add_keyword", py::overload_cast<const std::string&, std::string>(&atomsciflow::NWChem::py_add_keyword))
        .def("add_keyword", py::overload_cast<const std::string&, std::string, int>(&atomsciflow::NWChem::py_add_keyword))
        .def("add_keyword", py::overload_cast<const std::string&, std::string, double>(&atomsciflow::NWChem::py_add_keyword))
        .def("add_keyword", py::overload_cast<const std::string&, std::string, std::string>(&atomsciflow::NWChem::py_add_keyword))
        .def("new_directive", &atomsciflow::NWChem::new_directive)
        .def("py_set_field", py::overload_cast<const std::string&, int, int, int>(&atomsciflow::NWChem::py_set_field))
        .def("py_set_field", py::overload_cast<const std::string&, double, int, int>(&atomsciflow::NWChem::py_set_field))
        .def("py_set_field", py::overload_cast<const std::string&, std::string, int, int>(&atomsciflow::NWChem::py_set_field))
        .def("get_xyz", &atomsciflow::NWChem::get_xyz)
        .def("run", &atomsciflow::NWChem::run)
        .def_readwrite("job", &atomsciflow::NWChem::job)
        ;
}

PYBIND11_MODULE(nwchem, m) {
    m.doc() = "nwchem module";
    
    add_class_nwchem(m);
}
