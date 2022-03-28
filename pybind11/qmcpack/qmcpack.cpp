// @file pybind11/qmcpack/qmcpack.cpp
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Mon 14 Mar 2022 03:34:43 PM CST

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/qmcpack/input.h"
#include "atomsciflow/qmcpack/qmcpack.h"

namespace py = pybind11;

void add_class_input(py::module& m) {
    py::class_<atomsciflow::qmcpack::Input>(m, "Input")
        .def(py::init<>())
        .def("to_string", &atomsciflow::qmcpack::Input::to_string)
        .def("put_i_key_int", &atomsciflow::qmcpack::Input::put_i_key<int>)
        .def("put_i_key_float", &atomsciflow::qmcpack::Input::put_i_key<float>)
        .def("put_i_key_str", &atomsciflow::qmcpack::Input::put_i_key<std::string>)
        .def("put_i_key_attr_int", &atomsciflow::qmcpack::Input::put_i_key_attr<int>)
        .def("put_i_key_attr_float", &atomsciflow::qmcpack::Input::put_i_key_attr<float>)
        .def("put_i_key_attr_str", &atomsciflow::qmcpack::Input::put_i_key_attr<std::string>)
        .def("put_attr_int", &atomsciflow::qmcpack::Input::put_attr<int>)
        .def("put_attr_float", &atomsciflow::qmcpack::Input::put_attr<float>)
        .def("put_attr_str", &atomsciflow::qmcpack::Input::put_attr<std::string>)
        ;
}

void add_class_qmcpack(py::module& m) {
    py::class_<atomsciflow::Qmcpack>(m, "Qmcpack")
        .def(py::init<>())
        .def("get_xyz", &atomsciflow::Qmcpack::get_xyz)
        .def("to_string", &atomsciflow::Qmcpack::to_string)
        .def("set_job_steps_default", &atomsciflow::Qmcpack::set_job_steps_default)
        .def("run", &atomsciflow::Qmcpack::run)
        .def_readwrite("input", &atomsciflow::Qmcpack::input)
        ;
}

PYBIND11_MODULE(qmcpack, m) {
    m.doc() = "qmcpack module";
    
    add_class_qmcpack(m);
    add_class_input(m);
}

