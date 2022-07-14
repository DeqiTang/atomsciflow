
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/qe/pwscf.h"
#include "atomsciflow/qe/io/params.h"

namespace py = pybind11;

void add_class_namelist(py::module& m) {
    py::class_<atomsciflow::qe::Namelist>(m, "Namelist")
        .def(py::init<>())
        .def("to_string", py::overload_cast<>(&atomsciflow::qe::Namelist::to_string))
        .def("to_string", py::overload_cast<std::string>(&atomsciflow::qe::Namelist::to_string))
        .def_readwrite("name", &atomsciflow::qe::Namelist::name)
        .def_readwrite("status", &atomsciflow::qe::Namelist::status)
        .def_readwrite("type", &atomsciflow::qe::Namelist::type)
        ;
}

void add_class_pwscf(py::module& m) {
    py::class_<atomsciflow::qe::PwScf>(m, "PwScf")
        .def(py::init<>())
        .def("get_xyz", &atomsciflow::qe::PwScf::get_xyz)
        .def("to_string", [](atomsciflow::qe::PwScf& _this) {
            return _this.to_string("");
        })
        .def("set_param", py::overload_cast<const std::string&, std::string, int>(&atomsciflow::qe::PwScf::set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string, double>(&atomsciflow::qe::PwScf::set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string, std::string>(&atomsciflow::qe::PwScf::set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string, std::vector<int>>(&atomsciflow::qe::PwScf::set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string, std::vector<double>>(&atomsciflow::qe::PwScf::set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string, std::vector<std::string>>(&atomsciflow::qe::PwScf::set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string, std::vector<std::vector<int>>>(&atomsciflow::qe::PwScf::set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string, std::vector<std::vector<double>>>(&atomsciflow::qe::PwScf::set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string, std::vector<std::vector<std::string>>>(&atomsciflow::qe::PwScf::set_param))        
        .def("run", &atomsciflow::qe::PwScf::run)
        .def("set_kpoints", &atomsciflow::qe::PwScf::set_kpoints)
        .def("set_occupations", &atomsciflow::qe::PwScf::set_occupations)
        .def_readwrite("namelists", &atomsciflow::qe::PwScf::namelists)
        .def_readwrite("misc", &atomsciflow::qe::PwScf::misc)
        .def_readwrite("job", &atomsciflow::qe::PwScf::job)
        ;
}

void add_class_pwscfpseudo(py::module& m) {
    py::class_<atomsciflow::qe::PwScfPseudo>(m, "PwScfPseudo")
        .def("to_string", &atomsciflow::qe::PwScfPseudo::to_string)
        .def_readwrite("directory", &atomsciflow::qe::PwScfPseudo::directory)
        ;
}

void add_class_pwscfmisc(py::module& m) {
    py::class_<atomsciflow::qe::PwScfMisc>(m, "PwScfMisc")
        .def(py::init<>())
        .def("to_string", &atomsciflow::qe::PwScfMisc::to_string)
        .def("set_kpoints", &atomsciflow::qe::PwScfMisc::set_kpoints)
        .def("to_string_kpoints", &atomsciflow::qe::PwScfMisc::to_string_kpoints)
        .def("to_string_atomic_forces", &atomsciflow::qe::PwScfMisc::to_string_atomic_forces)
        .def("set_atomic_forces", &atomsciflow::qe::PwScfMisc::set_atomic_forces)
        .def("basic_setting", &atomsciflow::qe::PwScfMisc::basic_setting)
        .def_readwrite("xyz", &atomsciflow::qe::PwScfMisc::xyz)
        .def_readwrite("cell_params", &atomsciflow::qe::PwScfMisc::cell_params)
        .def_readwrite("pseudo", &atomsciflow::qe::PwScfMisc::pseudo)
        .def_readwrite("kpoints_option", &atomsciflow::qe::PwScfMisc::kpoints_option)
        .def_readwrite("kpoints_mp", &atomsciflow::qe::PwScfMisc::kpoints_mp)
        .def_readwrite("ifstatic", &atomsciflow::qe::PwScfMisc::ifstatic)
        .def_readwrite("atomic_forces_status", &atomsciflow::qe::PwScfMisc::atomic_forces_status)
        .def_readwrite("atomic_forces", &atomsciflow::qe::PwScfMisc::atomic_forces)
        .def_readwrite("crystal_b", &atomsciflow::qe::PwScfMisc::crystal_b)
        ;
}

void add_qe_read_params(py::module& m) {
    m.def("read_params", &atomsciflow::qe::io::read_params);
}

PYBIND11_MODULE(qe, m) {
    m.doc() = "qe module";
    m.attr("__version__") = "0.0.1";

    add_class_namelist(m);
    add_class_pwscfpseudo(m);
    add_class_pwscfmisc(m);
    add_class_pwscf(m);

    add_qe_read_params(m);
}
