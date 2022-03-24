
#include <iostream>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // needed for automatical handling with STL 

#include "atomsciflow/base/atom.h"
#include "atomsciflow/base/xyz.h"
#include "atomsciflow/cp2k/cp2k.h"
#include "atomsciflow/cp2k/static.h"
#include "atomsciflow/cp2k/opt.h"

namespace py = pybind11;

void add_class_cp2ksection(py::module& m) {
    py::class_<atomsciflow::Cp2kSection>(m, "Cp2kSection")
        .def(py::init<>())
        .def("to_string", [](atomsciflow::Cp2kSection& _this) {
            return _this.to_string();
        })
        .def("add_section", py::overload_cast<const std::string&>(&atomsciflow::Cp2kSection::add_section))
        .def("add_section", py::overload_cast<const std::string&, atomsciflow::Cp2kSection>(&atomsciflow::Cp2kSection::add_section))
        .def("remove_section", &atomsciflow::Cp2kSection::remove_section)
        .def("set_param", py::overload_cast<std::string, int>(&atomsciflow::Cp2kSection::set_param))
        .def("set_param", py::overload_cast<std::string, double>(&atomsciflow::Cp2kSection::set_param))
        .def("set_param", py::overload_cast<std::string, std::string>(&atomsciflow::Cp2kSection::set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<int>>(&atomsciflow::Cp2kSection::set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<double>>(&atomsciflow::Cp2kSection::set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::string>>(&atomsciflow::Cp2kSection::set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::vector<int>>>(&atomsciflow::Cp2kSection::set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::vector<double>>>(&atomsciflow::Cp2kSection::set_param))
        .def("set_param", py::overload_cast<std::string, std::vector<std::vector<std::string>>>(&atomsciflow::Cp2kSection::set_param))
        .def("contains", &atomsciflow::Cp2kSection::contains)
        .def("set_status", &atomsciflow::Cp2kSection::set_status)
        .def("remove_param", &atomsciflow::Cp2kSection::remove_param)
        .def("clear", &atomsciflow::Cp2kSection::clear)
        .def("get_int_0d", &atomsciflow::Cp2kSection::get<int>)
        .def_readwrite("sections", &atomsciflow::Cp2kSection::sections)
        ;
}

void add_class_cp2k(py::module& m) {
    py::class_<atomsciflow::Cp2k>(m, "Cp2k")
        .def(py::init<>())
        .def("new_section", &atomsciflow::Cp2k::new_section)
        .def("exists_section", &atomsciflow::Cp2k::exists_section)
        .def("to_string", &atomsciflow::Cp2k::to_string)
        .def("set_param", py::overload_cast<const std::string&, int>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, double>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<int>>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<double>>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::string>>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<int>>>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<double>>>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<std::string>>>(&atomsciflow::Cp2k::py_set_param))        
        // atomsciflow::Cp2k::set_subsys is a overloaded function
        // and atomsciflow::Cp2k::set_subsys() is private
        // we don't expose it to python. so only provide the following
        .def("set_subsys", py::overload_cast<atomsciflow::Xyz&>(&atomsciflow::Cp2k::set_subsys), py::return_value_policy::reference)
        .def("get_xyz", &atomsciflow::Cp2k::get_xyz)
        .def("run", &atomsciflow::Cp2k::run)
        .def_readwrite("sections", &atomsciflow::Cp2k::sections)
        .def_readwrite("job", &atomsciflow::Cp2k::job)
        ;
}

void add_class_cp2kstatic(py::module& m) {

    py::class_<atomsciflow::Cp2kStatic>(m, "Cp2kStatic")
        .def(py::init<>())
        .def("new_section", &atomsciflow::Cp2kStatic::new_section)  
        .def("exists_section", &atomsciflow::Cp2k::exists_section)       
        .def("to_string", &atomsciflow::Cp2kStatic::to_string)
        .def("set_param", py::overload_cast<const std::string&, int>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, double>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<int>>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<double>>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::string>>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<int>>>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<double>>>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<std::string>>>(&atomsciflow::Cp2k::py_set_param))         
        // atomsciflow::Cp2k::set_subsys is a overloaded function
        // and atomsciflow::Cp2k::set_subsys() is private
        // we don't expose it to python. so only provide the following
        .def("set_subsys", py::overload_cast<atomsciflow::Xyz&>(&atomsciflow::Cp2kStatic::set_subsys), py::return_value_policy::reference)
        .def("get_xyz", &atomsciflow::Cp2kStatic::get_xyz)
        .def("run", &atomsciflow::Cp2kStatic::run)
        .def_readwrite("sections", &atomsciflow::Cp2kStatic::sections)
        .def_readwrite("job", &atomsciflow::Cp2kStatic::job)
        ;
}

void add_class_cp2kopt(py::module& m) {

    py::class_<atomsciflow::Cp2kOpt>(m, "Cp2kOpt")
        .def(py::init<>())
        .def("new_section", &atomsciflow::Cp2kOpt::new_section)
        .def("exists_section", &atomsciflow::Cp2k::exists_section)
        .def("to_string", &atomsciflow::Cp2kOpt::to_string)
        .def("set_param", py::overload_cast<const std::string&, int>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, double>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::string>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<int>>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<double>>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::string>>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<int>>>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<double>>>(&atomsciflow::Cp2k::py_set_param))
        .def("set_param", py::overload_cast<const std::string&, std::vector<std::vector<std::string>>>(&atomsciflow::Cp2k::py_set_param))         
        // atomsciflow::Cp2k::set_subsys is a overloaded function
        // and atomsciflow::Cp2k::set_subsys() is private
        // we don't expose it to python. so only provide the following
        .def("set_subsys", py::overload_cast<atomsciflow::Xyz&>(&atomsciflow::Cp2kOpt::set_subsys), py::return_value_policy::reference)
        .def("get_xyz", &atomsciflow::Cp2kOpt::get_xyz)
        .def("run", &atomsciflow::Cp2kOpt::run)
        .def_readwrite("sections", &atomsciflow::Cp2kOpt::sections)
        .def_readwrite("job", &atomsciflow::Cp2kOpt::job)
        ;
}

PYBIND11_MODULE(cp2k, m) {
    m.doc() = "cpptest module";
    m.attr("__version__") = "0.0.0";

    add_class_cp2ksection(m);
    add_class_cp2k(m);   
    add_class_cp2kstatic(m);
    add_class_cp2kopt(m);
}

