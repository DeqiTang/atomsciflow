
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/vasp/vasp.h"
#include "atomsciflow/vasp/static.h"
#include "atomsciflow/vasp/post/phonopy.h"
#include "atomsciflow/server/job_scheduler.h"

namespace py = pybind11;

void add_class_vasp(py::module& m) {

    py::class_<atomsciflow::Vasp>(m, "Vasp")
        .def(py::init<>())
        .def("get_xyz", &atomsciflow::Vasp::get_xyz)
        .def("set_params", &atomsciflow::Vasp::set_params)
        .def("set_kpoints", &atomsciflow::Vasp::set_kpoints)
        .def("run", &atomsciflow::Vasp::run)
        .def_readwrite("job", &atomsciflow::Vasp::job)
        ;

}


void add_class_vaspstatic(py::module& m) {

    py::class_<atomsciflow::VaspStatic>(m, "VaspStatic")
        .def(py::init<>())
        .def("get_xyz", &atomsciflow::VaspStatic::get_xyz)
        .def("set_params", &atomsciflow::VaspStatic::set_params)
        .def("set_kpoints", &atomsciflow::VaspStatic::set_kpoints)
        .def("run", &atomsciflow::Vasp::run)
        .def_readwrite("job", &atomsciflow::VaspStatic::job)
        ;
}


void add_class_phonopypost(py::module& m) {
    
    py::class_<atomsciflow::PhonopyPost>(m, "PhonopyPost")
        .def(py::init<>())
        .def("get_kpath", &atomsciflow::PhonopyPost::get_kpath)
        .def("get_xyz", py::overload_cast<std::string&>(&atomsciflow::PhonopyPost::get_xyz))
        .def("get_xyz", py::overload_cast<atomsciflow::Xyz&>(&atomsciflow::PhonopyPost::get_xyz))
        .def("process", &atomsciflow::PhonopyPost::process)
        .def_readwrite("kpath", &atomsciflow::PhonopyPost::kpath)
        .def_readwrite("supercell_n", &atomsciflow::PhonopyPost::supercell_n)
        ;

}

PYBIND11_MODULE(vasp, m) {
    m.doc() = "vasp module";
    m.attr("__version__") = "0.0.1";

    add_class_vasp(m);
    add_class_vaspstatic(m);
    add_class_phonopypost(m);
}

