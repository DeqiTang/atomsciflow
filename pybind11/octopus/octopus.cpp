/************************************************************************
MIT License

Copyright (c) 2021 Deqi Tang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
************************************************************************/

/// @file pybind11/octopus/octopus.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 04:13:57 PM CST

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/octopus/octopus.h"
#include "atomsciflow/octopus/io/params.h"

namespace py = pybind11;

void add_class_octopus(py::module& m) {
    py::class_<atomsciflow::Octopus>(m, "Octopus")
        .def(py::init<>())        
        .def("to_string", &atomsciflow::Octopus::to_string)
        .def("set_param", py::overload_cast<const std::string&, int>(&atomsciflow::Octopus::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, double>(&atomsciflow::Octopus::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string>(&atomsciflow::Octopus::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<int>>(&atomsciflow::Octopus::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<double>>(&atomsciflow::Octopus::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::string>>(&atomsciflow::Octopus::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<int>>>(&atomsciflow::Octopus::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<double>>>(&atomsciflow::Octopus::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<std::string>>>(&atomsciflow::Octopus::py_set_param))        
        .def("new_block", &atomsciflow::Octopus::new_block)
        .def("get_xyz", &atomsciflow::Octopus::get_xyz)
        .def("run", &atomsciflow::Octopus::run)
        .def_readwrite("job", &atomsciflow::Octopus::job)
        ;
}

void add_octopus_read_params(py::module& m) {
    m.def("read_params", &atomsciflow::octopus::io::read_params);
}

PYBIND11_MODULE(octopus, m) {
    m.doc() = "octopus module";
    
    add_class_octopus(m);
    
    add_octopus_read_params(m);
}
