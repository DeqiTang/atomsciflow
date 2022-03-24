// @file pybind11/variable/variable.cpp
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Fri 07 Jan 2022 03:33:23 PM CST

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // needed for automatical handling with STL

#include "atomsciflow/variable/variable.h"
#include "atomsciflow/variable/variable_v2.h"
#include "atomsciflow/variable/group.h"
#include "atomsciflow/variable/group_v2.h"
#include "atomsciflow/variable/utils.h"


namespace py = pybind11;


void add_class_variable(py::module& m) {
    
    py::class_<atomsciflow::Variable>(m, "Variable")
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
        .def("set", py::overload_cast<std::string, int>(&atomsciflow::Variable::set))
        .def("set", py::overload_cast<std::string, double>(&atomsciflow::Variable::set))
        .def("set", py::overload_cast<std::string, std::string>(&atomsciflow::Variable::set))
        .def("set", py::overload_cast<std::string, std::vector<int>>(&atomsciflow::Variable::set))
        .def("set", py::overload_cast<std::string, std::vector<double>>(&atomsciflow::Variable::set))
        .def("set", py::overload_cast<std::string, std::vector<std::string>>(&atomsciflow::Variable::set))
        .def("set", py::overload_cast<std::string, std::vector<std::vector<int>>>(&atomsciflow::Variable::set))
        .def("set", py::overload_cast<std::string, std::vector<std::vector<double>>>(&atomsciflow::Variable::set))
        .def("set", py::overload_cast<std::string, std::vector<std::vector<std::string>>>(&atomsciflow::Variable::set))
        .def("to", py::overload_cast<int&>(&atomsciflow::Variable::to))
        .def("to", py::overload_cast<double&>(&atomsciflow::Variable::to))
        .def("to", py::overload_cast<std::string&>(&atomsciflow::Variable::to))
        .def("to", py::overload_cast<std::vector<int>&>(&atomsciflow::Variable::to))
        .def("to", py::overload_cast<std::vector<double>&>(&atomsciflow::Variable::to))
        .def("to", py::overload_cast<std::vector<std::string>&>(&atomsciflow::Variable::to))
        .def("to", py::overload_cast<std::vector<std::vector<int>>&>(&atomsciflow::Variable::to))
        .def("to", py::overload_cast<std::vector<std::vector<double>>&>(&atomsciflow::Variable::to))
        .def("to", py::overload_cast<std::vector<std::vector<std::string>>&>(&atomsciflow::Variable::to))
        .def("set_n", &atomsciflow::Variable::set_n)
        .def("to_string", py::overload_cast<>(&atomsciflow::Variable::to_string))
        .def("to_string", py::overload_cast<int>(&atomsciflow::Variable::to_string))
        .def("to_string", py::overload_cast<std::string, std::string>(&atomsciflow::Variable::to_string))
        .def("to_string", py::overload_cast<std::string, std::string, int>(&atomsciflow::Variable::to_string))
        ;
}


PYBIND11_MODULE(variable, m) {

    m.doc() = "variable module";
    m.attr("__version__") = "0.0.1";

    add_class_variable(m);

}
