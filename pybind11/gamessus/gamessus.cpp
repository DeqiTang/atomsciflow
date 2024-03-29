// @file pybind11/gamessus/gamessus.cpp
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Fri 11 Mar 2022 10:10:09 AM CST

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/gamessus/gamessus.h"
#include "atomsciflow/gamessus/static.h"
#include "atomsciflow/gamessus/opt.h"
#include "atomsciflow/gamessus/io/params.h"
#include "atomsciflow/gamessus/post/opt.h"

namespace py = pybind11;

void add_class_gamessus(py::module& m) {
    py::class_<atomsciflow::GamessUS>(m, "GamessUS")
        .def(py::init<>())
        .def("get_xyz", &atomsciflow::GamessUS::get_xyz)
        .def("to_string", &atomsciflow::GamessUS::to_string)
        .def("run", &atomsciflow::GamessUS::run)
        .def("set_param", py::overload_cast<const std::string&, int>(&atomsciflow::GamessUS::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, double>(&atomsciflow::GamessUS::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string>(&atomsciflow::GamessUS::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<int>>(&atomsciflow::GamessUS::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<double>>(&atomsciflow::GamessUS::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::string>>(&atomsciflow::GamessUS::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<int>>>(&atomsciflow::GamessUS::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<double>>>(&atomsciflow::GamessUS::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<std::string>>>(&atomsciflow::GamessUS::py_set_param))
        .def_readwrite("xyz", &atomsciflow::GamessUS::xyz)
        .def_readwrite("groups", &atomsciflow::GamessUS::groups)
        .def_readwrite("job", &atomsciflow::GamessUS::job)    
        ;
}

void add_class_gamessus_static(py::module& m) {
    py::class_<atomsciflow::GamessUSStatic>(m, "GamessUSStatic")
        .def(py::init<>())
        .def("get_xyz", &atomsciflow::GamessUSStatic::get_xyz)
        .def("to_string", &atomsciflow::GamessUSStatic::to_string)
        .def("run", &atomsciflow::GamessUSStatic::run)
        .def("set_param", py::overload_cast<const std::string&, int>(&atomsciflow::GamessUSStatic::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, double>(&atomsciflow::GamessUSStatic::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string>(&atomsciflow::GamessUSStatic::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<int>>(&atomsciflow::GamessUSStatic::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<double>>(&atomsciflow::GamessUSStatic::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::string>>(&atomsciflow::GamessUSStatic::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<int>>>(&atomsciflow::GamessUSStatic::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<double>>>(&atomsciflow::GamessUSStatic::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<std::string>>>(&atomsciflow::GamessUSStatic::py_set_param))
        .def_readwrite("xyz", &atomsciflow::GamessUS::xyz)
        .def_readwrite("groups", &atomsciflow::GamessUS::groups)
        .def_readwrite("job", &atomsciflow::GamessUS::job)         
        ;
}

void add_class_gamessus_opt(py::module& m) {
    py::class_<atomsciflow::GamessUSOpt>(m, "GamessUSOpt")
        .def(py::init<>())
        .def("get_xyz", &atomsciflow::GamessUSOpt::get_xyz)
        .def("to_string", &atomsciflow::GamessUSOpt::to_string)
        .def("run", &atomsciflow::GamessUSOpt::run)
        .def(py::init<>())
        .def("set_param", py::overload_cast<const std::string&, int>(&atomsciflow::GamessUS::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, double>(&atomsciflow::GamessUS::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string>(&atomsciflow::GamessUS::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<int>>(&atomsciflow::GamessUS::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<double>>(&atomsciflow::GamessUS::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::string>>(&atomsciflow::GamessUS::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<int>>>(&atomsciflow::GamessUS::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<double>>>(&atomsciflow::GamessUS::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<std::string>>>(&atomsciflow::GamessUS::py_set_param))
        .def_readwrite("xyz", &atomsciflow::GamessUS::xyz)
        .def_readwrite("groups", &atomsciflow::GamessUS::groups)
        .def_readwrite("job", &atomsciflow::GamessUS::job)         
        ;
}

void add_gamessus_read_params(py::module& m) {
    m.def("read_params", &atomsciflow::gamessus::io::read_params);
}

void add_class_post_opt(py::module& m) {
    py::class_<atomsciflow::gamessus::post::Opt>(m, "PostOpt")
        .def(py::init<>())
        .def("run", &atomsciflow::gamessus::post::Opt::run)
        ;
}

PYBIND11_MODULE(gamessus, m) {
    m.doc() = "gamessus module";
    
    add_class_gamessus(m);
    add_class_gamessus_static(m);
    add_class_gamessus_opt(m);

    add_gamessus_read_params(m);
    add_class_post_opt(m);
}
