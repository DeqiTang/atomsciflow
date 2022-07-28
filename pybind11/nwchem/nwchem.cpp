/// @file pybind11/nwchem/nwchem.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 04:13:51 PM CST

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/nwchem/directive.h"
#include "atomsciflow/nwchem/nwchem.h"
#include "atomsciflow/nwchem/io/params.h"

namespace py = pybind11;

void add_class_directive(py::module& m) {
    py::class_<atomsciflow::nwchem::Directive>(m, "Directive")
        .def(py::init<>())
        .def("to_string", &atomsciflow::nwchem::Directive::to_string)
        .def("set_keywords", &atomsciflow::nwchem::Directive::set_keyworkds)
        .def_readwrite("name", &atomsciflow::nwchem::Directive::name)
        .def_readwrite("keywords", &atomsciflow::nwchem::Directive::keywords)
        .def_readwrite("simple", &atomsciflow::nwchem::Directive::simple)
        .def_readwrite("fields", &atomsciflow::nwchem::Directive::fields)
        .def_readwrite("directives", &atomsciflow::nwchem::Directive::directives)
        ;
}

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
        .def("set_field", py::overload_cast<const std::string&, int, int, int>(&atomsciflow::NWChem::py_set_field))
        .def("set_field", py::overload_cast<const std::string&, double, int, int>(&atomsciflow::NWChem::py_set_field))
        .def("set_field", py::overload_cast<const std::string&, std::string, int, int>(&atomsciflow::NWChem::py_set_field))
        .def("set_keywords", py::overload_cast<const std::string&, const std::vector<int>&>(&atomsciflow::NWChem::py_set_keywords))
        .def("set_keywords", py::overload_cast<const std::string&, const std::vector<double>&>(&atomsciflow::NWChem::py_set_keywords))
        .def("set_keywords", py::overload_cast<const std::string&, const std::vector<std::string>&>(&atomsciflow::NWChem::py_set_keywords))
        .def("get_xyz", &atomsciflow::NWChem::get_xyz)
        .def("set_simple", &atomsciflow::NWChem::set_simple)
        .def("run", &atomsciflow::NWChem::run)
        .def_readwrite("job", &atomsciflow::NWChem::job)
        .def_readwrite("directives", &atomsciflow::NWChem::directives)
        ;
}

void add_nwchem_read_params(py::module& m) {
    m.def("read_params", atomsciflow::nwchem::io::read_params);
}

PYBIND11_MODULE(nwchem, m) {
    m.doc() = "nwchem module";
    
    add_class_directive(m); 
    add_class_nwchem(m);

    add_nwchem_read_params(m);
}
