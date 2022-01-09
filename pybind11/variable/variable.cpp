// @file pybind11/variable/variable.cpp
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Fri 07 Jan 2022 03:33:23 PM CST

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // needed for automatical handling with STL

#include "atomsciflow/variable/variable_v1.h"
#include "atomsciflow/variable/variable_v2.h"
#include "atomsciflow/variable/group_v1.h"
#include "atomsciflow/variable/group_v2.h"
#include "atomsciflow/variable/utils.h"


namespace py = pybind11;


void add_class_variable(py::module& m) {
    
    py::class_<atomsciflow::VariableV1>(m, "VariableV1")
        .def(py::init<>())
        .def(py::init<std::string, int>())
        .def(py::init<std::string, double>())
        .def(py::init<std::string, std::string>())
        .def(py::init<std::string, std::vector<int>>())
        .def(py::init<std::string, std::vector<double>>())
        .def(py::init<std::string, std::vector<std::string>>())
        .def(py::init<std::string, std::vector<std::vector<int>>>())
        .def(py::init<std::string, std::vector<std::vector<double>>>())
        .def(py::init<std::string, std::vector<std::vector<std::string>>>())
        .def("set", py::overload_cast<std::string, int>(&atomsciflow::VariableV1::set))
        .def("set", py::overload_cast<std::string, double>(&atomsciflow::VariableV1::set))
        .def("set", py::overload_cast<std::string, std::string>(&atomsciflow::VariableV1::set))
        .def("set", py::overload_cast<std::string, std::vector<int>>(&atomsciflow::VariableV1::set))
        .def("set", py::overload_cast<std::string, std::vector<double>>(&atomsciflow::VariableV1::set))
        .def("set", py::overload_cast<std::string, std::vector<std::string>>(&atomsciflow::VariableV1::set))
        .def("set", py::overload_cast<std::string, std::vector<std::vector<int>>>(&atomsciflow::VariableV1::set))
        .def("set", py::overload_cast<std::string, std::vector<std::vector<double>>>(&atomsciflow::VariableV1::set))
        .def("set", py::overload_cast<std::string, std::vector<std::vector<std::string>>>(&atomsciflow::VariableV1::set))
        .def("to", py::overload_cast<int&>(&atomsciflow::VariableV1::to))
        .def("to", py::overload_cast<double&>(&atomsciflow::VariableV1::to))
        .def("to", py::overload_cast<std::string&>(&atomsciflow::VariableV1::to))
        .def("to", py::overload_cast<std::vector<int>&>(&atomsciflow::VariableV1::to))
        .def("to", py::overload_cast<std::vector<double>&>(&atomsciflow::VariableV1::to))
        .def("to", py::overload_cast<std::vector<std::string>&>(&atomsciflow::VariableV1::to))
        .def("to", py::overload_cast<std::vector<std::vector<int>>&>(&atomsciflow::VariableV1::to))
        .def("to", py::overload_cast<std::vector<std::vector<double>>&>(&atomsciflow::VariableV1::to))
        .def("to", py::overload_cast<std::vector<std::vector<std::string>>&>(&atomsciflow::VariableV1::to))
        .def("set_n", &atomsciflow::VariableV1::set_n)
        .def("to_string", py::overload_cast<>(&atomsciflow::VariableV1::to_string))
        .def("to_string", py::overload_cast<int>(&atomsciflow::VariableV1::to_string))
        .def("to_string", py::overload_cast<std::string, std::string>(&atomsciflow::VariableV1::to_string))
        .def("to_string", py::overload_cast<std::string, std::string, int>(&atomsciflow::VariableV1::to_string))

        ;
    
}


PYBIND11_MODULE(variable, m) {

    m.doc() = "variable module";
    m.attr("__version__") = "0.0.1";

    add_class_variable(m);

}
