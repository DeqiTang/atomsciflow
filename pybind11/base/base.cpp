#include <string>
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // for automatical c++ stl support

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/base/xyz.h"
#include "atomsciflow/base/atomic_radius.h"
#include "atomsciflow/base/kpath.h"

namespace py = pybind11;

void add_class_atom(py::module& m) {

    py::class_<atomsciflow::Atom>(m, "Atom")
        .def(py::init<>())
        .def("get_name", &atomsciflow::Atom::get_name)
        .def("get_x", &atomsciflow::Atom::get_x)
        .def("get_y", &atomsciflow::Atom::get_y)
        .def("get_z", &atomsciflow::Atom::get_z)
        .def("set_name", &atomsciflow::Atom::set_name)
        .def("set_x", &atomsciflow::Atom::set_x)
        .def("set_y", &atomsciflow::Atom::set_y)
        .def("set_z", &atomsciflow::Atom::set_z)
        .def("set_xyz", &atomsciflow::Atom::set_xyz)
        .def_property("name", &atomsciflow::Atom::get_name, &atomsciflow::Atom::set_name)
        .def_readwrite("x", &atomsciflow::Atom::x)
        .def_readwrite("y", &atomsciflow::Atom::y)
        .def_readwrite("z", &atomsciflow::Atom::z)
        .def_readwrite("name", &atomsciflow::Atom::name)
        ;
}

void add_class_xyz(py::module& m) {
    py::class_<atomsciflow::Xyz>(m, "Xyz")
        .def(py::init<>())
        .def("read_xyz_file", &atomsciflow::Xyz::read_xyz_file)
        .def("write_xyz_file", &atomsciflow::Xyz::write_xyz_file)
        .def("get_cell", &atomsciflow::Xyz::get_cell)
        .def("get_atoms", &atomsciflow::Xyz::get_atoms)
        .def("cartesian", &atomsciflow::Xyz::cartesian)
        .def("get_fractional", &atomsciflow::Xyz::get_fractional)
        .def("volume", &atomsciflow::Xyz::volume)
        .def("natom", &atomsciflow::Xyz::natom)
        .def_readwrite("file", &atomsciflow::Xyz::file)
        .def_readwrite("nspecies", &atomsciflow::Xyz::nspecies)
        .def_readwrite("elements_set", &atomsciflow::Xyz::elements_set)
        .def_readwrite("atoms", &atomsciflow::Xyz::atoms)
        .def_readwrite("cell", &atomsciflow::Xyz::cell)
        ;
}

void add_class_crystal(py::module& m) {
    py::class_<atomsciflow::Crystal>(m, "Crystal")
        .def(py::init<>())
        .def("read_xyz_file", &atomsciflow::Crystal::read_xyz_file)
        .def("write_xyz_file", &atomsciflow::Crystal::write_xyz_file)
        .def("read_xyz_str", &atomsciflow::Crystal::read_xyz_str)
        .def("write_xyz_str", py::overload_cast<std::string&>(&atomsciflow::Crystal::write_xyz_str))
        .def("write_xyz_str", py::overload_cast<>(&atomsciflow::Crystal::write_xyz_str))
        .def("get_atoms", &atomsciflow::Crystal::get_atoms)
        //.def("get_cell_atoms", &atomsciflow::Crystal::get_cell_atoms)
        // atomsciflow::Crystal::get_cell_atoms is a overloaded function
        // we need to use py::overload_cast to deal with it
        // however, int atomsciflow::Crystal::get_cell_atoms(double**, Atom*)
        // the double pointer is not automatically handled in pybind11
        // so we ignore the overload, but use a lambda function to deal with it
        .def("get_cell_atoms", [](atomsciflow::Crystal& _this, std::vector<std::vector<double>>& cell, atomsciflow::Atom* atoms) {
            return _this.get_cell_atoms(cell, atoms);
        })
        .def("cartesian", &atomsciflow::Crystal::cartesian)
        .def("get_fractional", &atomsciflow::Crystal::get_fractional)
        .def("volume", &atomsciflow::Crystal::volume)
        .def("build_supercell", &atomsciflow::Crystal::build_supercell)
        .def("to_base_xyz", &atomsciflow::Crystal::to_base_xyz)
        .def("remove_atom", &atomsciflow::Crystal::remove_atom)
        .def("remove_atoms", &atomsciflow::Crystal::remove_atoms)
        .def("natom", &atomsciflow::Crystal::natom)
        .def_readwrite("atoms", &atomsciflow::Crystal::atoms)
        ; 
}


void add_class_atomic_radius(py::module& m) {

    py::class_<atomsciflow::AtomicRadius>(m, "AtomicRadius")
        .def(py::init<>())
        .def_readwrite("emprical", &atomsciflow::AtomicRadius::emprical)
        .def_readwrite("calculated", &atomsciflow::AtomicRadius::calculated)
        .def_readwrite("vanderwaals", &atomsciflow::AtomicRadius::vanderwaals)
        .def_readwrite("covalent_single_bond", &atomsciflow::AtomicRadius::covalent_single_bond)
        .def_readwrite("covalent_triple_bond", &atomsciflow::AtomicRadius::covalent_triple_bond)
        ;
}

void add_class_kpath(py::module& m) {
    m.doc() = "atomsciflow kpath module";

    py::class_<atomsciflow::Kpath>(m, "Kpath")
        .def(py::init<>())
        .def_readwrite("coords", &atomsciflow::Kpath::coords)
        .def_readwrite("labels", &atomsciflow::Kpath::labels)
        .def_readwrite("links", &atomsciflow::Kpath::links)
        ;
}

void add_func_get_kpath(py::module& m) {
    m.def("get_kpath", &atomsciflow::get_kpath);
}

PYBIND11_MODULE(base, m) {
    m.doc() = "base module";
    m.attr("__version__") = "0.0.1";

    add_class_atom(m);
    add_class_xyz(m);
    add_class_crystal(m);
    add_class_atomic_radius(m);
    add_class_kpath(m);

    add_func_get_kpath(m);
}
