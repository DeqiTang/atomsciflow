
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
        .def("set_job_steps_default", &atomsciflow::Vasp::set_job_steps_default)
        .def("run", &atomsciflow::Vasp::run)
        .def_readwrite("job", &atomsciflow::Vasp::job)
        .def_readwrite("config", &atomsciflow::Vasp::config)
        .def_readwrite("incar", &atomsciflow::Vasp::incar)
        .def_readwrite("poscar", &atomsciflow::Vasp::poscar)
        .def_readwrite("kpoints", &atomsciflow::Vasp::kpoints)
        ;
}

void add_class_vaspincar(py::module& m) {
    // atomsciflow::Vasp::incar is a std::shared_ptr to VaspIncar
    // if we will binding the member to Vasp in Python and use it
    // We need to bind the VaspIncar class, and set the holder type
    // to std::shared_ptr<atomsciflow::VaspIncar>, like this:
    py::class_<atomsciflow::VaspIncar, std::shared_ptr<atomsciflow::VaspIncar>>(m, "VaspIncar")
        .def(py::init<>())
        .def("to_string", &atomsciflow::VaspIncar::to_string)
        .def("basic_setting", &atomsciflow::VaspIncar::basic_setting)
        .def("set_runtype", &atomsciflow::VaspIncar::set_runtype)
        .def("set_md", &atomsciflow::VaspIncar::set_md)
        ;
}

void add_class_vaspposcar(py::module& m) {
    py::class_<atomsciflow::VaspPoscar, std::shared_ptr<atomsciflow::VaspPoscar>>(m, "VaspPoscar")
        .def(py::init<>())
        .def("to_string", &atomsciflow::VaspPoscar::to_string)
        .def("get_xyz", &atomsciflow::VaspPoscar::get_xyz)
        ;
}

void add_class_vaspkpoints(py::module& m) {
    py::class_<atomsciflow::VaspKpoints, std::shared_ptr<atomsciflow::VaspKpoints>>(m, "VaspKpoints")
        .def(py::init<>())
        .def("to_string", &atomsciflow::VaspKpoints::to_string)
        .def("set_kpoints", &atomsciflow::VaspKpoints::set_kpoints)
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
    add_class_vaspincar(m);
    add_class_vaspposcar(m);
    add_class_vaspkpoints(m);
    add_class_phonopypost(m);
}

