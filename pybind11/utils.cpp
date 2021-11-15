/**
 * Copyright (c) 2021 DeqiTang
 * Distributed under the terms of the MIT license
 * @author: DeqiTang
 * email: deqi_tang@163.com 
 */

#include <iostream>
#include <pybind11/pybind11.h>
// for automatic recognization an conversion of vector map arguments
#include <pybind11/stl.h>

#include "atomsciflow/utils.h"

namespace py = pybind11;

PYBIND11_MODULE(utils, m) {
    m.doc() = "atomsciflow utils module";
    m.def("version", &atomsciflow::version, py::return_value_policy::reference);
}
