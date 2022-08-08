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
#include "atomsciflow/elk/io/params.h"
#include "atomsciflow/elk/post/opt.h"

namespace py = pybind11;

void add_class_elk(py::module& m) {
    py::class_<atomsciflow::elk::Elk>(m, "Elk")
        .def(py::init<>())
        .def("get_xyz", &atomsciflow::elk::Elk::get_xyz)
        .def("to_string", &atomsciflow::elk::Elk::to_string)
        .def("set_param", py::overload_cast<const std::string&, int>(&atomsciflow::elk::Elk::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, double>(&atomsciflow::elk::Elk::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string>(&atomsciflow::elk::Elk::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<int>>(&atomsciflow::elk::Elk::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<double>>(&atomsciflow::elk::Elk::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::string>>(&atomsciflow::elk::Elk::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<int>>>(&atomsciflow::elk::Elk::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<double>>>(&atomsciflow::elk::Elk::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<std::string>>>(&atomsciflow::elk::Elk::py_set_param))        
        .def("set_kpath", &atomsciflow::elk::Elk::set_kpath)
        .def("run", &atomsciflow::elk::Elk::run)
        .def_readwrite("job", &atomsciflow::elk::Elk::job)
        ;
}

void add_class_elkstatic(py::module& m) {
    py::class_<atomsciflow::elk::Static>(m, "Static")
        .def(py::init<>())
        .def("to_string", &atomsciflow::elk::Static::to_string)
        .def_readwrite("job", &atomsciflow::elk::Static::job)
        ;
}

void add_class_elkopt(py::module& m) {
    py::class_<atomsciflow::elk::Opt>(m, "Opt")
        .def(py::init<>())
        .def("to_string", &atomsciflow::elk::Opt::to_string)
        .def_readwrite("job", &atomsciflow::elk::Opt::job)
        ;
}

void add_elk_read_params(py::module& m) {
    m.def("read_params", &atomsciflow::elk::io::read_params);
}

void add_class_post_opt(py::module& m) {
    py::class_<atomsciflow::elk::post::Opt>(m, "PostOpt")
        .def(py::init<>())
        .def("run", &atomsciflow::elk::post::Opt::run)
        ;
}

PYBIND11_MODULE(elk, m) {
    m.doc() = "elk module";
    
    add_class_elk(m);
    add_class_elkstatic(m);
    add_class_elkopt(m);

    add_elk_read_params(m);

    add_class_post_opt(m);
}

