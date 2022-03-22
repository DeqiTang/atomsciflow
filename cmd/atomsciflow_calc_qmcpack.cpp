// @file cmd/atomsciflow_calc_qmcpack.cpp
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Sun 13 Mar 2022 04:05:33 PM CST

#include "atomsciflow_calc_qmcpack.h"

#include <boost/program_options.hpp>
//#include <filesystem>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include <regex>
#include <cstdlib>

namespace po = boost::program_options;

//namespace fs = std::filesystem;
namespace fs = boost::filesystem;     // --std=c++11 -lboost_filesystem -lboost_system

void atomsciflow_calc_qmcpack(po::parsed_options& parsed, po::variables_map& vm) {

    // convert subcommand has the following options:
    po::options_description opt_qmcpack("qmcpack options");
    opt_qmcpack.add_options()
        ("help, h", "Print out help information for cp2k sub command")
        //("input, i", po::value<std::string>()->required(), "input structure file")
        ("xyz", po::value<std::string>()->required(), "input xyz structure file")
        ("directory, d", po::value<std::string>()->default_value("asflow-calc-running"), "The directory to put all the resources")
    ;
    std::vector<std::string> opts = po::collect_unrecognized(parsed.options, po::include_positional);
    opts.erase(opts.begin());
    //parse again...
    po::store(po::command_line_parser(opts).options(opt_qmcpack).style(po::command_line_style::unix_style | po::command_line_style::allow_long_disguise).extra_style_parser(&allow_negative_numbers).run(), vm);

    if (vm.count("help")) {
        std::cout << opt_qmcpack << std::endl;
        std::exit(1);
    }
    po::notify(vm);

    std::cout << "working direrctory: " << vm["directory"].as<std::string>() << std::endl;
    auto task = new atomsciflow::Qmcpack{};
    task->get_xyz(vm["xyz"].as<std::string>());
    //std::cout << task->to_string() << "\n";
    task->run(vm["directory"].as<std::string>());
    delete task;
}

