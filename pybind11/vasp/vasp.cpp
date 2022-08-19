
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/vasp/vasp.h"
#include "atomsciflow/vasp/static.h"
#include "atomsciflow/vasp/post/phonopy.h"
#include "atomsciflow/vasp/post/bands.h"
#include "atomsciflow/server/job_scheduler.h"
#include "atomsciflow/vasp/io/params.h"
#include "atomsciflow/vasp/phonopy.h"
#include "atomsciflow/vasp/post/dos.h"

namespace py = pybind11;

void add_class_vasp(py::module& m) {

    py::class_<atomsciflow::Vasp>(m, "Vasp")
        .def(py::init<>())
        .def("get_xyz", &atomsciflow::Vasp::get_xyz)
        .def("set_param", py::overload_cast<std::string, int>(&atomsciflow::Vasp::py_set_param))
        .def("set_param", py::overload_cast<std::string, double>(&atomsciflow::Vasp::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::string>(&atomsciflow::Vasp::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<int>>(&atomsciflow::Vasp::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<double>>(&atomsciflow::Vasp::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::string>>(&atomsciflow::Vasp::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::vector<int>>>(&atomsciflow::Vasp::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::vector<double>>>(&atomsciflow::Vasp::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::vector<std::string>>>(&atomsciflow::Vasp::py_set_param))
        .def("set_params", &atomsciflow::Vasp::set_params)
        .def("set_kpoints", &atomsciflow::Vasp::set_kpoints)
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
        .def_readwrite("elem_natom_in_number_order", &atomsciflow::VaspPoscar::elem_natom_in_number_order)
        ;
}

void add_class_vaspkpoints(py::module& m) {
    py::class_<atomsciflow::VaspKpoints, std::shared_ptr<atomsciflow::VaspKpoints>>(m, "VaspKpoints")
        .def(py::init<>())
        .def("to_string", &atomsciflow::VaspKpoints::to_string)
        .def("set_kpoints", &atomsciflow::VaspKpoints::set_kpoints)
        ;
}

void add_class_post_phonopy(py::module& m) {
    
    py::class_<atomsciflow::vasp::post::Phonopy>(m, "PostPhonopy")
        .def(py::init<>())
        .def("run", &atomsciflow::vasp::post::Phonopy::run)
        .def("set_kpath", &atomsciflow::vasp::post::Phonopy::set_kpath)
        .def("extract_data", &atomsciflow::vasp::post::Phonopy::extract_data)
        ;
}

void add_vasp_read_params(py::module& m) {
    m.def("read_params", &atomsciflow::vasp::io::read_params);
}

void add_class_phonopy(py::module& m) {

    py::class_<atomsciflow::vasp::Phonopy>(m, "Phonopy")
        .def(py::init<>())
        .def("get_xyz", &atomsciflow::vasp::Phonopy::get_xyz)
        .def("set_param", py::overload_cast<std::string, int>(&atomsciflow::vasp::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<std::string, double>(&atomsciflow::vasp::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::string>(&atomsciflow::vasp::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<int>>(&atomsciflow::vasp::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<double>>(&atomsciflow::vasp::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::string>>(&atomsciflow::vasp::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::vector<int>>>(&atomsciflow::vasp::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::vector<double>>>(&atomsciflow::vasp::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::vector<std::string>>>(&atomsciflow::vasp::Phonopy::py_set_param))
        .def("set_params", &atomsciflow::vasp::Phonopy::set_params)
        .def("set_kpoints", &atomsciflow::vasp::Phonopy::set_kpoints)
        .def("run", &atomsciflow::vasp::Phonopy::run)
        .def_readwrite("job", &atomsciflow::vasp::Phonopy::job)
        .def_readwrite("config", &atomsciflow::vasp::Phonopy::config)
        .def_readwrite("incar", &atomsciflow::vasp::Phonopy::incar)
        .def_readwrite("poscar", &atomsciflow::vasp::Phonopy::poscar)
        .def_readwrite("kpoints", &atomsciflow::vasp::Phonopy::kpoints)
        ;
}

void add_class_post_band(py::module& m) {
    py::class_<atomsciflow::vasp::post::Bands>(m, "PostBand")
        .def(py::init<>())
        .def("set_kpath", &atomsciflow::vasp::post::Bands::set_kpath)
        .def("run", &atomsciflow::vasp::post::Bands::run)
        ;
}

void add_class_post_dos(py::module& m) {
    py::class_<atomsciflow::vasp::post::Dos>(m, "PostDos")
        .def(py::init<>())
        .def("run", &atomsciflow::vasp::post::Dos::run)
        ;
}

PYBIND11_MODULE(vasp, m) {
    m.doc() = "vasp module";
    m.attr("__version__") = "0.0.1";

    add_class_vasp(m);
    add_class_vaspincar(m);
    add_class_vaspposcar(m);
    add_class_vaspkpoints(m);
    add_class_post_phonopy(m);

    add_vasp_read_params(m);

    add_class_phonopy(m);
    add_class_post_band(m);
    add_class_post_dos(m);
}
