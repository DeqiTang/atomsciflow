// @file atomsciflow_calc_qe.h
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Mon 24 Jan 2022 12:01:07 AM CST

#ifndef CMD_ATOMSCIFLOW_CALC_QE_H_
#define CMD_ATOMSCIFLOW_CALC_QE_H_
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
#include "atomsciflow/utils.h"
#include "atomsciflow/abinit/abinit.h"
#include "atomsciflow/cp2k/cp2k.h"
#include "atomsciflow/cp2k/opt.h"
#include "atomsciflow/qe/pw.h"

#include "cmd_utils.h"


void atomsciflow_calc_qe(boost::program_options::parsed_options& parsed, boost::program_options::variables_map& vm);

#endif // CMD_ATOMSCIFLOW_CALC_QE_H_
