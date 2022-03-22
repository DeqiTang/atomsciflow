// @file pybind11/config/config.cpp
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Sat 05 Mar 2022 08:33:55 PM CST

#include <string>
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // for automatical c++ stl support

#include "atomsciflow/config/config_manager.h"

namespace py = pybind11;

void add_class_config_manager(py::module& m) {

    py::class_<atomsciflow::ConfigManager>(m, "ConfigManager")
        .def(py::init<>())
        ;

}


PYBIND11_MODULE(config, m) {
    m.doc() = "config module";
    m.attr("__version__") = "0.0.0";

    add_class_config_manager(m);
}


