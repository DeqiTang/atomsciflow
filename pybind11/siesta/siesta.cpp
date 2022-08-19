/// @file pybind11/siesta/siesta.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Thu 24 Mar 2022 09:35:01 PM CST

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/siesta/siesta.h"
#include "atomsciflow/siesta/opt.h"
#include "atomsciflow/siesta/io/params.h"
#include "atomsciflow/siesta/phonopy.h"
#include "atomsciflow/siesta/post/phonopy.h"
#include "atomsciflow/siesta/post/dos.h"

namespace py = pybind11;

void add_class_siesta(py::module& m) {
    py::class_<atomsciflow::Siesta>(m, "Siesta")
        .def(py::init<>())
        .def("to_string", &atomsciflow::Siesta::to_string)
        .def("set_param", py::overload_cast<std::string, int>(&atomsciflow::Siesta::py_set_param))
        .def("set_param", py::overload_cast<std::string, double>(&atomsciflow::Siesta::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::string>(&atomsciflow::Siesta::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<int>>(&atomsciflow::Siesta::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<double>>(&atomsciflow::Siesta::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::string>>(&atomsciflow::Siesta::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::vector<int>>>(&atomsciflow::Siesta::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::vector<double>>>(&atomsciflow::Siesta::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::vector<std::string>>>(&atomsciflow::Siesta::py_set_param))
        .def("set_block_data", py::overload_cast<const std::string&, int, int, int>(&atomsciflow::Siesta::py_set_block_data))
        .def("set_block_data", py::overload_cast<const std::string&, double, int, int>(&atomsciflow::Siesta::py_set_block_data))
        .def("set_block_data", py::overload_cast<const std::string&, std::string, int, int>(&atomsciflow::Siesta::py_set_block_data))
        .def("set_block_data", py::overload_cast<const std::string&, std::vector<int>, int>(&atomsciflow::Siesta::py_set_block_data))
        .def("set_block_data", py::overload_cast<const std::string&, std::vector<double>, int>(&atomsciflow::Siesta::py_set_block_data))
        .def("set_block_data", py::overload_cast<const std::string&, std::vector<std::string>, int>(&atomsciflow::Siesta::py_set_block_data))
        .def("set_block_data", py::overload_cast<const std::string&, std::vector<std::vector<int>>>(&atomsciflow::Siesta::py_set_block_data))
        .def("set_block_data", py::overload_cast<const std::string&, std::vector<std::vector<double>>>(&atomsciflow::Siesta::py_set_block_data))
        .def("set_block_data", py::overload_cast<const std::string&, std::vector<std::vector<std::string>>>(&atomsciflow::Siesta::py_set_block_data))
        .def("set_block_data_size", &atomsciflow::Siesta::set_block_data_size)
        .def("set_status", &atomsciflow::Siesta::set_status)
        .def("set_block_status", &atomsciflow::Siesta::set_block_status)
        .def("get_xyz", &atomsciflow::Siesta::get_xyz)
        .def("set_bandlines", &atomsciflow::Siesta::set_bandlines)
        .def("run", &atomsciflow::Siesta::run)
        .def_readwrite("job", &atomsciflow::Siesta::job)
        ;
}

void add_class_siestaopt(py::module& m) {
    py::class_<atomsciflow::SiestaOpt>(m, "SiestaOpt")
        .def(py::init<>())
        .def("to_string", &atomsciflow::SiestaOpt::to_string)
        .def("get_xyz", &atomsciflow::SiestaOpt::get_xyz)
        .def("run", &atomsciflow::SiestaOpt::run)
        .def_readwrite("job", &atomsciflow::SiestaOpt::job)
        .def_readwrite("config", &atomsciflow::SiestaOpt::config)
        ;
}

void add_siesta_read_params(py::module& m) {
    m.def("read_params", &atomsciflow::siesta::io::read_params);
}

void add_class_phonopy(py::module& m) {
    py::class_<atomsciflow::siesta::Phonopy, atomsciflow::Siesta>(m, "Phonopy")
        .def(py::init<>())
        .def("to_string", &atomsciflow::siesta::Phonopy::to_string)
        .def("set_param", py::overload_cast<std::string, int>(&atomsciflow::siesta::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<std::string, double>(&atomsciflow::siesta::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::string>(&atomsciflow::siesta::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<int>>(&atomsciflow::siesta::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<double>>(&atomsciflow::siesta::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::string>>(&atomsciflow::siesta::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::vector<int>>>(&atomsciflow::siesta::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::vector<double>>>(&atomsciflow::siesta::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::vector<std::string>>>(&atomsciflow::siesta::Phonopy::py_set_param))
        .def("get_xyz", &atomsciflow::siesta::Phonopy::get_xyz)
        .def("run", &atomsciflow::siesta::Phonopy::run)
        .def_readwrite("job", &atomsciflow::siesta::Phonopy::job)
        ;
}

void add_class_post_phonopy(py::module& m) {
    py::class_<atomsciflow::siesta::post::Phonopy>(m, "PostPhonopy")
        .def(py::init<>())
        .def("run", &atomsciflow::siesta::post::Phonopy::run)
        .def("set_kpath", &atomsciflow::siesta::post::Phonopy::set_kpath)
        ;
}

void add_class_post_dos(py::module& m) {
    py::class_<atomsciflow::siesta::post::Dos>(m, "PostDos")
        .def(py::init<>())
        .def("run", &atomsciflow::siesta::post::Dos::run)
        ;
}

PYBIND11_MODULE(siesta, m) {
    m.doc() = "siesta module";
    m.attr("__version__") = "0.0.0";

    add_class_siesta(m);
    add_class_siestaopt(m);

    add_siesta_read_params(m);

    add_class_phonopy(m);
    add_class_post_phonopy(m);
    add_class_post_dos(m);
}
