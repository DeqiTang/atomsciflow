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


#include <boost/program_options.hpp>
//#include <filesystem>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include <regex>
#include <cstdlib>
#include "atomsciflow/utils.h"

#include "atomsciflow_calc_abinit.h"
#include "atomsciflow_calc_cp2k.h"
#include "atomsciflow_calc_qe.h"
#include "atomsciflow_calc_vasp.h"
#include "atomsciflow_calc_gamessus.h"
#include "atomsciflow_calc_elk.h"
#include "atomsciflow_calc_qmcpack.h"
#include "atomsciflow/cmd/cmd_utils.h"

namespace po = boost::program_options;


//namespace fs = std::filesystem;
namespace fs = boost::filesystem;     // --std=c++11 -lboost_filesystem -lboost_system


int main(int argc, char const* argv[]) {
    //
    po::options_description global("Global options");
    global.add_options()
        ("subcommand", po::value<std::string>(), "subcommand to execute")
        ("subargs", po::value<std::vector<std::string> >(), "Arguments for command")
        ("help, h", "print out help information");
        
    po::positional_options_description pos;
    pos.add("subcommand", 1).add("subargs", -1);
    
    po::variables_map vm;
    
    po::parsed_options parsed = po::command_line_parser(argc, argv).
        options(global).
        positional(pos).
        allow_unregistered().
        run();
        
    po::store(parsed, vm);

    log_cmd_start("atomsciflow-calc.x");
    
    if (0 == vm.count("subcommand")) { // or by vm.empty()
        std::cout << "You didn't specify any subcommand!\n";
        std::exit(1);
    }
    std::string subcommand = vm["subcommand"].as<std::string>();
    

    if ("abinit" == subcommand) {
        //
        log_sub_cmd_start(subcommand);
        
        atomsciflow_calc_abinit(parsed, vm);
        
        log_sub_cmd_end(subcommand);
        
    } else if ("cp2k" == subcommand) {
    
        log_sub_cmd_start(subcommand);
        
        atomsciflow_calc_cp2k(parsed, vm); 

        log_sub_cmd_end(subcommand); 

    } else if ("qe" == subcommand) {
        //
        log_sub_cmd_start(subcommand);
        
        atomsciflow_calc_qe(parsed, vm);        

        log_sub_cmd_end(subcommand); 

    } else if ("vasp" == subcommand) {

        log_sub_cmd_start(subcommand);

        atomsciflow_calc_vasp(parsed, vm);

        log_sub_cmd_end(subcommand);

    } else if ("gamessus" == subcommand) {
        
        log_sub_cmd_start(subcommand);
        
        atomsciflow_calc_gamessus(parsed, vm);

        log_sub_cmd_end(subcommand);

    } else if ("elk" == subcommand) {
        
        log_sub_cmd_start(subcommand);

        atomsciflow_calc_elk(parsed, vm);

        log_sub_cmd_end(subcommand);

    } else if ("qmcpack" == subcommand) {
        
        log_sub_cmd_start(subcommand);

        atomsciflow_calc_qmcpack(parsed, vm);

        log_sub_cmd_end(subcommand);

    } else {
        std::cout << "The specified subcommand is not defined!\n";
    }
        
    return 0;
}

