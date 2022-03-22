// @file cmd/atomsciflow_calc_qmcpack.h
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Sun 13 Mar 2022 04:04:46 PM CST

#include <boost/program_options.hpp>
//#include <filesystem>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include <regex>
#include <cstdlib>
#include "atomsciflow/parser/cif.h"
#include "atomsciflow/parser/xyz.h"
#include "atomsciflow/parser/tools.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/qmcpack/qmcpack.h"

#include "atomsciflow/cmd/cmd_utils.h"

void atomsciflow_calc_qmcpack(boost::program_options::parsed_options& parsed, boost::program_options::variables_map& vm);

