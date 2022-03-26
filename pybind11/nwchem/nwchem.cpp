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
        .def("set_param", py::overload_cast<const std::string&, int>(&atomsciflow::NWChem::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, double>(&atomsciflow::NWChem::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string>(&atomsciflow::NWChem::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<int>>(&atomsciflow::NWChem::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<double>>(&atomsciflow::NWChem::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::string>>(&atomsciflow::NWChem::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<int>>>(&atomsciflow::NWChem::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<double>>>(&atomsciflow::NWChem::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<std::string>>>(&atomsciflow::NWChem::py_set_param))        
        .def("new_directive", &atomsciflow::NWChem::new_directive)
        .def("get_xyz", &atomsciflow::NWChem::get_xyz)
        .def("run", &atomsciflow::NWChem::run)
        .def_readwrite("job", &atomsciflow::NWChem::job)
        ;
}

PYBIND11_MODULE(nwchem, m) {
    m.doc() = "nwchem module";
    
    add_class_nwchem(m);
}
