/// Copyright (c) 2021 DeqiTang
/// Distributed under the terms of the MIT license
/// @author: DeqiTang
/// email: deqi_tang@163.com 

#include <iostream>
#include <pybind11/pybind11.h>
// for automatic recognization an conversion of vector map arguments
#include <pybind11/stl.h>

#include "atomsciflow/utils.h"

namespace py = pybind11;

PYBIND11_MODULE(utils, m) {
    m.doc() = "atomsciflow utils module";
    m.def("version", &atomsciflow::version, py::return_value_policy::reference);

    m.def("move_along", &atomsciflow::move_along);
    m.def("remove_atoms", &atomsciflow::remove_atoms);
    m.def("vacuum_layer", &atomsciflow::vacuum_layer);
    m.def("set_frac_min_to_zero", &atomsciflow::set_frac_min_to_zero);
    m.def("set_frac_within_zero_and_one", &atomsciflow::set_frac_within_zero_and_one);
    m.def("inverse_geo_center", &atomsciflow::inverse_geo_center);
    m.def("inverse_point", &atomsciflow::inverse_point);
    m.def("inverse_cell_center", &atomsciflow::inverse_cell_center);
    m.def("rotate_along_axis", &atomsciflow::rotate_along_axis);
    m.def("enlarge_atoms", &atomsciflow::enlarge_atoms);
    m.def("enlarge_atoms_new_cell", &atomsciflow::enlarge_atoms_new_cell);
    m.def("redefine_lattice", &atomsciflow::redefine_lattice);
    m.def("cleave_surface", &atomsciflow::cleave_surface);
    m.def("merge_layers", &atomsciflow::merge_layers);
    m.def("build_nanotube_ab", &atomsciflow::build_nanotube_ab);
    m.def("build_nanotube_ac", &atomsciflow::build_nanotube_ac);
    m.def("build_nanotube_bc", &atomsciflow::build_nanotube_bc);

}
