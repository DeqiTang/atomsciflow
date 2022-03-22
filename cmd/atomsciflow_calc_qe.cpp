/************************************************************************
MIT License

Copyright (c) 2021 Deqi Tang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
************************************************************************/


// @file atomsciflow_calc_qe.cpp
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Mon 24 Jan 2022 12:01:33 AM CST

#include "atomsciflow_calc_qe.h"

#include <iostream>
#include <string>
#include <regex>
#include <cstdlib>
#include "atomsciflow/parser/cif.h"
#include "atomsciflow/parser/xyz.h"
#include "atomsciflow/parser/tools.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/utils.h"
#include "atomsciflow/qe/pwscf.h"

namespace po = boost::program_options;


//namespace fs = std::filesystem;
namespace fs = boost::filesystem;     // --std=c++11 -lboost_filesystem -lboost_system
    

void atomsciflow_calc_qe(po::parsed_options& parsed, po::variables_map& vm) {

    // convert command has the following options:
    po::options_description opt_qe("qe options");
    opt_qe.add_options()
        ("help, h", "Print out help information for qe subcommand")
        //("input, i", po::value<std::string>()->required(), "input structure file")
        ("xyz", po::value<std::string>(), "input xyz structure file")
        ("directory, d", po::value<std::string>()->default_value("asflow-calc-running"))
        ;
    //opts.add_options()
    //    ("input, i", po::value<std::string>(&input), "input structure file")
    //    ("output, o", po::value<std::string>(&output), "output structure file");
    // collect all the unrecognized options from the first pass. this will include the 
    // (positional) command name so we need to erase that
    std::vector<std::string> opts = po::collect_unrecognized(parsed.options, po::include_positional);
    opts.erase(opts.begin());
    //parse again...
    po::store(po::command_line_parser(opts).options(opt_qe).style(po::command_line_style::unix_style | po::command_line_style::allow_long_disguise).extra_style_parser(&allow_negative_numbers).run(), vm);

    if (vm.count("help")) {
        std::cout << opt_qe << std::endl;
        std::exit(1);
    }
    po::notify(vm);

    fs::path xyz_file(vm["xyz"].as<std::string>());

    std::cout << "working directory: " << vm["directory"].as<std::string>() << std::endl;
    auto task = new atomsciflow::qe::PwScf{};
    task->get_xyz(xyz_file.string());
    //std::cout << task->to_string() << std::endl;
    task->run(vm["directory"].as<std::string>());
    delete task;
}

