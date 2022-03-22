// @file pybind11/cmd/cmd.cpp
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Sat 05 Mar 2022 12:25:39 PM CST

#include <string>
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // for automatical c++ stl support

#include "atomsciflow/cmd/cmd_utils.h"

namespace py = pybind11;



PYBIND11_MODULE(cmd, m) {
    m.doc() = "cmd module";
    m.attr("__version__") = "0.0.1";

    m.def("log_cmd_start", &log_cmd_start);
    m.def("log_sub_cmd_start", &log_sub_cmd_start);
    m.def("log_sub_cmd_end", &log_sub_cmd_end);
    
}
