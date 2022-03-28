// @author DeqiTang
// Email: deqitang@gmail.com 

#include <string>
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // for automatical c++ stl support

#include "atomsciflow/config/config_manager.h"

namespace py = pybind11;

void add_class_config_manager(py::module& m) {

    py::class_<atomsciflow::ConfigManager, std::shared_ptr<atomsciflow::ConfigManager>>(m, "ConfigManager")
        .def(py::init<>())
        .def("get_home_dir", &atomsciflow::ConfigManager::get_home_dir)
        .def("get_config_dir", &atomsciflow::ConfigManager::get_config_dir)
        .def("get_pseudo_pot_dir", &atomsciflow::ConfigManager::get_pseudo_pot_dir)
        ;
}

PYBIND11_MODULE(config, m) {
    m.doc() = "config module";
    m.attr("__version__") = "0.0.0";

    add_class_config_manager(m);
}


