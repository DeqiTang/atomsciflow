
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/xyz.h"
#include "atomsciflow/cp2k/cp2k.h"
#include "atomsciflow/cp2k/static.h"
#include "atomsciflow/cp2k/opt.h"
#include "atomsciflow/cp2k/phonopy.h"
#include "atomsciflow/cp2k/post/opt.h"
#include "atomsciflow/cp2k/post/vcopt.h"
#include "atomsciflow/cp2k/post/md.h"
#include "atomsciflow/cp2k/post/phonopy.h"
#include "atomsciflow/cp2k/post/pdos.h"
#include "atomsciflow/cp2k/post/bands.h"
#include "atomsciflow/cp2k/io/params.h"


namespace py = pybind11;

void add_class_cp2ksection(py::module& m) {
    py::class_<atomsciflow::cp2k::Cp2kSection>(m, "Cp2kSection")
        .def(py::init<>())
        .def("to_string", [](atomsciflow::cp2k::Cp2kSection& _this) {
            return _this.to_string();
        })
        .def("add_section", py::overload_cast<const std::string&>(&atomsciflow::cp2k::Cp2kSection::add_section))
        .def("add_section", py::overload_cast<const std::string&, const std::shared_ptr<atomsciflow::cp2k::Cp2kSection>&>(&atomsciflow::cp2k::Cp2kSection::add_section))
        .def("remove_section", &atomsciflow::cp2k::Cp2kSection::remove_section)
        .def("set_param", py::overload_cast<std::string, int>(&atomsciflow::cp2k::Cp2kSection::set_param))
        .def("set_param", py::overload_cast<std::string, double>(&atomsciflow::cp2k::Cp2kSection::set_param))
        .def("set_param", py::overload_cast<std::string, std::string>(&atomsciflow::cp2k::Cp2kSection::set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<int>>(&atomsciflow::cp2k::Cp2kSection::set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<double>>(&atomsciflow::cp2k::Cp2kSection::set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::string>>(&atomsciflow::cp2k::Cp2kSection::set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::vector<int>>>(&atomsciflow::cp2k::Cp2kSection::set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::vector<double>>>(&atomsciflow::cp2k::Cp2kSection::set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::vector<std::string>>>(&atomsciflow::cp2k::Cp2kSection::set_param))
        .def("contains", &atomsciflow::cp2k::Cp2kSection::contains)
        .def("set_status", &atomsciflow::cp2k::Cp2kSection::set_status)
        .def("remove_param", &atomsciflow::cp2k::Cp2kSection::remove_param)
        .def("clear", &atomsciflow::cp2k::Cp2kSection::clear)
        .def("get_int_0d", &atomsciflow::cp2k::Cp2kSection::get<int>)
        .def_readwrite("sections", &atomsciflow::cp2k::Cp2kSection::sections)
        ;
}

void add_class_cp2k(py::module& m) {
    py::class_<atomsciflow::cp2k::Cp2k>(m, "Cp2k")
        .def(py::init<>())
        .def("new_section", &atomsciflow::cp2k::Cp2k::new_section)
        .def("exists_section", &atomsciflow::cp2k::Cp2k::exists_section)
        .def("to_string", &atomsciflow::cp2k::Cp2k::to_string)
        .def("set_param", py::overload_cast<const std::string&, int>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, double>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<int>>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<double>>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::string>>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<int>>>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<double>>>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<std::string>>>(&atomsciflow::cp2k::Cp2k::py_set_param))        
        // atomsciflow::cp2k::Cp2k::set_subsys is a overloaded function
        // and atomsciflow::cp2k::Cp2k::set_subsys() is private
        // we don't expose it to python. so only provide the following
        //.def("set_subsys", py::overload_cast<atomsciflow::Xyz&>(&atomsciflow::cp2k::Cp2k::set_subsys), py::return_value_policy::reference)
        .def("set_subsys", py::overload_cast<atomsciflow::Xyz&>(&atomsciflow::cp2k::Cp2k::set_subsys))
        .def("set_kpoint_set", &atomsciflow::cp2k::Cp2k::set_kpoint_set)
        .def("get_xyz", &atomsciflow::cp2k::Cp2k::get_xyz)
        .def("run", &atomsciflow::cp2k::Cp2k::run)
        .def_readwrite("sections", &atomsciflow::cp2k::Cp2k::sections)
        .def_readwrite("job", &atomsciflow::cp2k::Cp2k::job)
        .def_readwrite("plumed", &atomsciflow::cp2k::Cp2k::plumed)
        ;
}

void add_class_cp2k_static(py::module& m) {

    py::class_<atomsciflow::cp2k::Static, atomsciflow::cp2k::Cp2k>(m, "Static")
        .def(py::init<>())
        .def("new_section", &atomsciflow::cp2k::Static::new_section)  
        .def("exists_section", &atomsciflow::cp2k::Static::exists_section)       
        .def("to_string", &atomsciflow::cp2k::Static::to_string)
        .def("set_param", py::overload_cast<const std::string&, int>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, double>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<int>>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<double>>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::string>>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<int>>>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<double>>>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<std::string>>>(&atomsciflow::cp2k::Cp2k::py_set_param))         
        // atomsciflow::cp2k::set_subsys is a overloaded function
        // and atomsciflow::cp2k::set_subsys() is private
        // we don't expose it to python. so only provide the following
        //.def("set_subsys", py::overload_cast<atomsciflow::Xyz&>(&atomsciflow::cp2k::Static::set_subsys), py::return_value_policy::reference)
        .def("set_subsys", py::overload_cast<atomsciflow::Xyz&>(&atomsciflow::cp2k::Static::set_subsys))
        .def("get_xyz", &atomsciflow::cp2k::Static::get_xyz)
        .def("run", &atomsciflow::cp2k::Static::run)
        .def_readwrite("sections", &atomsciflow::cp2k::Static::sections)
        .def_readwrite("job", &atomsciflow::cp2k::Static::job)
        ;
}

void add_class_cp2k_opt(py::module& m) {

    py::class_<atomsciflow::cp2k::Opt, atomsciflow::cp2k::Cp2k>(m, "Opt")
        .def(py::init<>())
        .def("new_section", &atomsciflow::cp2k::Opt::new_section)
        .def("exists_section", &atomsciflow::cp2k::Opt::exists_section)
        .def("to_string", &atomsciflow::cp2k::Opt::to_string)
        .def("set_param", py::overload_cast<const std::string&, int>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, double>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<int>>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<double>>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::string>>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<int>>>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<double>>>(&atomsciflow::cp2k::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<std::string>>>(&atomsciflow::cp2k::Cp2k::py_set_param))         
        // atomsciflow::cp2k::set_subsys is a overloaded function
        // and atomsciflow::cp2k::set_subsys() is private
        // we don't expose it to python. so only provide the following
        //.def("set_subsys", py::overload_cast<atomsciflow::Xyz&>(&atomsciflow::cp2k::Opt::set_subsys), py::return_value_policy::reference)
        .def("set_subsys", py::overload_cast<atomsciflow::Xyz&>(&atomsciflow::cp2k::Opt::set_subsys))
        .def("get_xyz", &atomsciflow::cp2k::Opt::get_xyz)
        .def("run", &atomsciflow::cp2k::Opt::run)
        .def_readwrite("sections", &atomsciflow::cp2k::Opt::sections)
        .def_readwrite("job", &atomsciflow::cp2k::Opt::job)
        ;
}

void add_class_cp2k_phonopy(py::module& m) {
    py::class_<atomsciflow::cp2k::Phonopy, atomsciflow::cp2k::Cp2k>(m, "Phonopy")
        .def(py::init<>())
        .def("new_section", &atomsciflow::cp2k::Phonopy::new_section)
        .def("exists_section", &atomsciflow::cp2k::Phonopy::exists_section)
        .def("to_string", &atomsciflow::cp2k::Phonopy::to_string)
        .def("set_param", py::overload_cast<const std::string&, int>(&atomsciflow::cp2k::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, double>(&atomsciflow::cp2k::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string>(&atomsciflow::cp2k::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<int>>(&atomsciflow::cp2k::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<double>>(&atomsciflow::cp2k::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::string>>(&atomsciflow::cp2k::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<int>>>(&atomsciflow::cp2k::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<double>>>(&atomsciflow::cp2k::Phonopy::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<std::string>>>(&atomsciflow::cp2k::Phonopy::py_set_param))        
        // atomsciflow::cp2k::Phonopy::set_subsys is a overloaded function
        // and atomsciflow::cp2k::Phonopy::set_subsys() is private
        // we don't expose it to python. so only provide the following
        //.def("set_subsys", py::overload_cast<atomsciflow::Xyz&>(&atomsciflow::cp2k::Phonopy::set_subsys), py::return_value_policy::reference)
        .def("set_subsys", py::overload_cast<atomsciflow::Xyz&>(&atomsciflow::cp2k::Phonopy::set_subsys))
        .def("get_xyz", &atomsciflow::cp2k::Phonopy::get_xyz)
        .def("run", &atomsciflow::cp2k::Phonopy::run)
        .def_readwrite("sections", &atomsciflow::cp2k::Phonopy::sections)
        .def_readwrite("job", &atomsciflow::cp2k::Phonopy::job)
        ;
}

void add_class_post_opt(py::module& m) {
    py::class_<atomsciflow::cp2k::post::Opt>(m, "PostOpt")
        .def(py::init<>())
        .def("read", &atomsciflow::cp2k::post::Opt::read)
        .def("run", &atomsciflow::cp2k::post::Opt::run)
        ;
}

void add_class_post_vcopt(py::module& m) {
    py::class_<atomsciflow::cp2k::post::VcOpt>(m, "PostVcOpt")
        .def(py::init<>())
        .def("read", &atomsciflow::cp2k::post::VcOpt::read)
        .def("run", &atomsciflow::cp2k::post::VcOpt::run)
        ;
}

void add_class_post_md(py::module& m) {
    py::class_<atomsciflow::cp2k::post::MD>(m, "PostMD")
        .def(py::init<>())
        .def("read", &atomsciflow::cp2k::post::MD::read)
        .def("run", &atomsciflow::cp2k::post::MD::run)
        ;
}

void add_class_post_phonopy(py::module& m) {
    py::class_<atomsciflow::cp2k::post::Phonopy>(m, "PostPhonopy")
        .def(py::init<>())
        .def("run", &atomsciflow::cp2k::post::Phonopy::run)
        .def("set_kpath", &atomsciflow::cp2k::post::Phonopy::set_kpath)
        ;
}

void add_class_post_pdos(py::module& m) {
    py::class_<atomsciflow::cp2k::post::Pdos>(m, "PostPdos")
        .def(py::init<>())
        .def("run", &atomsciflow::cp2k::post::Pdos::run)
        ;
}

void add_class_post_bands(py::module& m) {
    py::class_<atomsciflow::cp2k::post::Bands>(m, "PostBands")
        .def(py::init<>())
        .def("run", &atomsciflow::cp2k::post::Bands::run)
        .def("set_kpath", &atomsciflow::cp2k::post::Bands::set_kpath)
        ;
}

void add_func_read_params(py::module& m) {
    m.def("read_params", atomsciflow::cp2k::io::read_params);
}

PYBIND11_MODULE(cp2k, m) {
    m.doc() = "cp2k module";
    m.attr("__version__") = "0.0.0";

    add_class_cp2ksection(m);
    add_class_cp2k(m);

    add_class_cp2k_static(m);
    add_class_cp2k_opt(m);
    add_class_cp2k_phonopy(m);

    add_class_post_opt(m);
    add_class_post_vcopt(m);
    add_class_post_md(m);
    add_class_post_phonopy(m);
    add_class_post_pdos(m);
    add_class_post_bands(m);

    add_func_read_params(m);
}
