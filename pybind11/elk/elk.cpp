// @file pybind11/elk/elk.cpp
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Sat 12 Mar 2022 06:25:29 PM CST

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/elk/elk.h"
#include "atomsciflow/elk/static.h"
#include "atomsciflow/elk/opt.h"

namespace py = pybind11;

void add_class_elk(py::module& m) {
    py::class_<atomsciflow::Elk>(m, "Elk")
        .def(py::init<>())
        .def("get_xyz", &atomsciflow::Elk::get_xyz)
        .def("to_string", &atomsciflow::Elk::to_string)
        .def("set_param", py::overload_cast<const std::string&, std::string, int>(&atomsciflow::Elk::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string, double>(&atomsciflow::Elk::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string, std::string>(&atomsciflow::Elk::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string, std::vector<int>>(&atomsciflow::Elk::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string, std::vector<double>>(&atomsciflow::Elk::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string, std::vector<std::string>>(&atomsciflow::Elk::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string, std::vector<std::vector<int>>>(&atomsciflow::Elk::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string, std::vector<std::vector<double>>>(&atomsciflow::Elk::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string, std::vector<std::vector<std::string>>>(&atomsciflow::Elk::py_set_param))        
        .def("run", &atomsciflow::Elk::run)
        .def_readwrite("job", &atomsciflow::Elk::job)
        ;
}

void add_class_elkstatic(py::module& m) {
    py::class_<atomsciflow::ElkStatic>(m, "ElkStatic")
        .def(py::init<>())
        .def("to_string", &atomsciflow::ElkStatic::to_string)
        .def_readwrite("job", &atomsciflow::ElkStatic::job)
        ;
}

void add_class_elkopt(py::module& m) {
    py::class_<atomsciflow::ElkOpt>(m, "ElkOpt")
        .def(py::init<>())
        .def("to_string", &atomsciflow::ElkOpt::to_string)
        .def_readwrite("job", &atomsciflow::ElkOpt::job)
        ;
}

PYBIND11_MODULE(elk, m) {
    m.doc() = "elk module";
    
    add_class_elk(m);
    add_class_elkstatic(m);
    add_class_elkopt(m);
}

