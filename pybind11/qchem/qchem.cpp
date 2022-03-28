/// @file pybind11/qchem/qchem.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 04:14:10 PM CST

#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/qchem/qchem.h"

namespace py = pybind11;

void add_class_qchem(py::module& m) {
    py::class_<atomsciflow::QChem>(m, "QChem")
        .def(py::init<>())
        .def("to_string", &atomsciflow::QChem::to_string)
        .def("new_section", &atomsciflow::QChem::new_section)
        .def("get_xyz", &atomsciflow::QChem::get_xyz)
        .def("set_job_steps_default", &atomsciflow::QChem::set_job_steps_default)
        .def("run", &atomsciflow::QChem::run)
        .def_readwrite("job", &atomsciflow::QChem::job)        
        ;
}

PYBIND11_MODULE(qchem, m) {
    m.doc() = "qchem module";
    
    add_class_qchem(m);
}