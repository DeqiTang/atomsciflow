// @file pybind11/parser/parser.cpp
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Thu 06 Jan 2022 08:06:57 PM CST

#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL

#include "atomsciflow/parser/cif.h"
#include "atomsciflow/parser/cube.h"
#include "atomsciflow/parser/tools.h"
#include "atomsciflow/parser/xyz.h"
#include "atomsciflow/parser/xyztraj.h"

namespace py = pybind11;

void add_class_cube_electron_density(py::module& m) {

    py::class_<atomsciflow::CubeElectronDensity>(m, "CubeElectrondensity")
        .def(py::init<>())
        .def("read_cube", &atomsciflow::CubeElectronDensity::read_cube)
        ;
}

PYBIND11_MODULE(parser, m) {

    m.doc() = "atomsciflow parser module";
    m.def("read_cif_file", &atomsciflow::read_cif_file);
    m.def("write_cif_file", &atomsciflow::write_cif_file);

    add_class_cube_electron_density(m);
   
    m.def("read_structure_file", &atomsciflow::read_structure_file);

    
    m.def("read_xyz_file", &atomsciflow::read_xyz_file);
    m.def("write_xyz_file", &atomsciflow::write_xyz_file);
    
    m.def("read_xyztraj_file", &atomsciflow::read_xyztraj_file);
    m.def("write_xyztraj_file", &atomsciflow::write_xyztraj_file);

}
