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
#include "atomsciflow/nwchem/post/opt.h"

namespace py = pybind11;

void add_class_directive(py::module& m) {
    py::class_<atomsciflow::nwchem::Directive>(m, "Directive")
        .def(py::init<>())
        .def("to_string", &atomsciflow::nwchem::Directive::to_string)
        .def("set_keywords", &atomsciflow::nwchem::Directive::set_keyworkds)
        .def_readwrite("name", &atomsciflow::nwchem::Directive::name)
        .def_readwrite("keywords", &atomsciflow::nwchem::Directive::keywords)
        .def_readwrite("simple", &atomsciflow::nwchem::Directive::simple)
        .def_readwrite("status", &atomsciflow::nwchem::Directive::status)
        .def_readwrite("fields", &atomsciflow::nwchem::Directive::fields)
        .def_readwrite("directives", &atomsciflow::nwchem::Directive::directives)
        ;
}

void add_class_nwchem(py::module& m) {
    py::class_<atomsciflow::nwchem::NWChem>(m, "NWChem")
        .def(py::init<>())        
        .def("to_string", &atomsciflow::nwchem::NWChem::to_string)
        .def("add_keyword", py::overload_cast<const std::string&, int>(&atomsciflow::nwchem::NWChem::py_add_keyword))
        .def("add_keyword", py::overload_cast<const std::string&, double>(&atomsciflow::nwchem::NWChem::py_add_keyword))
        .def("add_keyword", py::overload_cast<const std::string&, std::string>(&atomsciflow::nwchem::NWChem::py_add_keyword))
        .def("add_keyword", py::overload_cast<const std::string&, int, int>(&atomsciflow::nwchem::NWChem::py_add_keyword))
        .def("add_keyword", py::overload_cast<const std::string&, double, double>(&atomsciflow::nwchem::NWChem::py_add_keyword))
        .def("add_keyword", py::overload_cast<const std::string&, std::string, std::string>(&atomsciflow::nwchem::NWChem::py_add_keyword))
        .def("new_directive", &atomsciflow::nwchem::NWChem::new_directive)
        .def("set_field", py::overload_cast<const std::string&, int, int, int>(&atomsciflow::nwchem::NWChem::py_set_field))
        .def("set_field", py::overload_cast<const std::string&, double, int, int>(&atomsciflow::nwchem::NWChem::py_set_field))
        .def("set_field", py::overload_cast<const std::string&, std::string, int, int>(&atomsciflow::nwchem::NWChem::py_set_field))
        .def("set_keywords", py::overload_cast<const std::string&, const std::vector<int>&>(&atomsciflow::nwchem::NWChem::py_set_keywords))
        .def("set_keywords", py::overload_cast<const std::string&, const std::vector<double>&>(&atomsciflow::nwchem::NWChem::py_set_keywords))
        .def("set_keywords", py::overload_cast<const std::string&, const std::vector<std::string>&>(&atomsciflow::nwchem::NWChem::py_set_keywords))
        .def("get_xyz", &atomsciflow::nwchem::NWChem::get_xyz)
        .def("set_simple", &atomsciflow::nwchem::NWChem::set_simple)
        .def("set_status", &atomsciflow::nwchem::NWChem::set_status)
        .def("run", &atomsciflow::nwchem::NWChem::run)
        .def_readwrite("job", &atomsciflow::nwchem::NWChem::job)
        .def_readwrite("xyz", &atomsciflow::nwchem::NWChem::xyz)
        .def_readwrite("tasks", &atomsciflow::nwchem::NWChem::tasks)
        ;
}

void add_nwchem_read_params(py::module& m) {
    m.def("read_params", atomsciflow::nwchem::io::read_params);
}

void add_class_post_opt(py::module& m) {
    py::class_<atomsciflow::nwchem::post::Opt>(m, "PostOpt")
        .def(py::init<>())
        .def("run", &atomsciflow::nwchem::post::Opt::run)
        ;
}

PYBIND11_MODULE(nwchem, m) {
    m.doc() = "nwchem module";
    
    add_class_directive(m); 
    add_class_nwchem(m);

    add_nwchem_read_params(m);
    add_class_post_opt(m);
}
