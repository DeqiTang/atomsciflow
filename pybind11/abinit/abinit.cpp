// @file pybind11/abinit/abinit.cpp
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Thu 06 Jan 2022 07:50:41 PM CST

#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL

#include "atomsciflow/abinit/abinit.h"

namespace py = pybind11;

void add_class_abinit(py::module& m) {

    py::class_<atomsciflow::Abinit>(m, "Abinit")
        .def(py::init<>())
        .def("get_xyz", &atomsciflow::Abinit::get_xyz)
        .def("to_string", &atomsciflow::Abinit::to_string)

        .def("set_param", py::overload_cast<std::string, int>(&atomsciflow::Abinit::py_set_param))
        .def("set_param", py::overload_cast<std::string, double>(&atomsciflow::Abinit::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::string>(&atomsciflow::Abinit::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<int>>(&atomsciflow::Abinit::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<double>>(&atomsciflow::Abinit::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::string>>(&atomsciflow::Abinit::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::vector<int>>>(&atomsciflow::Abinit::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::vector<double>>>(&atomsciflow::Abinit::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::vector<std::string>>>(&atomsciflow::Abinit::py_set_param))
        .def("set_params", py::overload_cast<std::map<std::string, int>&>(&atomsciflow::Abinit::py_set_params))
        .def("set_params", py::overload_cast<std::map<std::string, double>&>(&atomsciflow::Abinit::py_set_params))
        .def("set_params", py::overload_cast<std::map<std::string, std::string>&>(&atomsciflow::Abinit::py_set_params))
        .def("set_params", py::overload_cast<std::map<std::string, std::vector<int>>&>(&atomsciflow::Abinit::py_set_params))
        .def("set_params", py::overload_cast<std::map<std::string, std::vector<double>>&>(&atomsciflow::Abinit::py_set_params))
        .def("set_params", py::overload_cast<std::map<std::string, std::vector<std::string>>&>(&atomsciflow::Abinit::py_set_params))
        .def("set_params", py::overload_cast<std::map<std::string, std::vector<std::vector<int>>>&>(&atomsciflow::Abinit::py_set_params))
        .def("set_params", py::overload_cast<std::map<std::string, std::vector<std::vector<double>>>&>(&atomsciflow::Abinit::py_set_params))
        .def("set_params", py::overload_cast<std::map<std::string, std::vector<std::vector<std::string>>>&>(&atomsciflow::Abinit::py_set_params))
        .def("set_job_steps_default", &atomsciflow::Abinit::set_job_steps_default)
        .def("run", &atomsciflow::Abinit::run)
        .def_readwrite("job", &atomsciflow::Abinit::job)
        ;
}

PYBIND11_MODULE(abinit, m) {
    m.doc() = "abinit module";
    
    add_class_abinit(m);
}

