// @file pybind11/server/server.cpp
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Thu 17 Mar 2022 09:02:12 PM CST

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/server/job_scheduler.h"

namespace py = pybind11;

void add_class_job_scheduler(py::module& m) {
    
    py::class_<atomsciflow::JobScheduler>(m, "JobScheduler")
        .def(py::init<>())
        .def("set_run", py::overload_cast<std::string, std::string>(&atomsciflow::JobScheduler::py_set_run))
        .def("set_run", py::overload_cast<std::string, int>(&atomsciflow::JobScheduler::py_set_run))
        .def("set_run", py::overload_cast<std::string, float>(&atomsciflow::JobScheduler::py_set_run))
        .def("set_run", py::overload_cast<std::string, double>(&atomsciflow::JobScheduler::py_set_run))
        .def("set_run_default", &atomsciflow::JobScheduler::set_run_default)
        .def("gen_llhpc", &atomsciflow::JobScheduler::gen_llhpc)
        .def("gen_yh", &atomsciflow::JobScheduler::gen_yh)
        .def("gen_pbs", &atomsciflow::JobScheduler::gen_pbs)
        .def("gen_bash", &atomsciflow::JobScheduler::gen_bash)
        .def("gen_lsf_sz", &atomsciflow::JobScheduler::gen_lsf_sz)
        .def("gen_lsf_sustc", &atomsciflow::JobScheduler::gen_lsf_sustc)
        .def("gen_cdcloud", &atomsciflow::JobScheduler::gen_cdcloud)
        ;
}

PYBIND11_MODULE(server, m) {
    m.doc() = "server module";
    m.attr("__version__") = "0.0.1";

    add_class_job_scheduler(m);
}

