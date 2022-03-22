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


/**
 * Copyright (c) 2021 DeqiTang
 * Distributed under the terms of the MIT license
 * @author: DeqiTang
 * email: deqi_tang@163.com 
 */

#include <iostream>
#include <boost/program_options.hpp>
//#include <experimental/filesystem>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include <regex>
#include <cstdlib>

#include <yaml-cpp/yaml.h>

#include "atomsciflow/cmd/cmd_utils.h"

namespace po = boost::program_options;

//namespace filesys = std::experimental::filesystem;  // --std=c++17 -lstdc++fs
namespace filesys = boost::filesystem;     // --std=c++11 -lboost_filesystem -lboost_system


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
   
    log_cmd_start("atomsciflow-yaml.x");
    
    if (0 == vm.count("subcommand")) { // or by vm.empty()
        std::cout << "You didn't specify any subcommand!\n";
        std::exit(1);
    }
    std::string subcommand = vm["subcommand"].as<std::string>();


    if (subcommand == "info") {
    
        log_sub_cmd_start("info");//
        
        // exec subcommand has the following options:
        po::options_description opt_exec("exec options");
        opt_exec.add_options()
            ("help, h", "Print out help information for exec subcommand")
            ("input, i", po::value<std::string>()->required(), "file path of the yaml file")
            ;
        // collect all the unrecognized options from the first pass. this will include the 
        // (positional) sub command name so we need to erase that
        std::vector<std::string> opts = po::collect_unrecognized(parsed.options, po::include_positional);
        opts.erase(opts.begin());
        //parse again...
        po::store(po::command_line_parser(opts).options(opt_exec).style(po::command_line_style::unix_style | po::command_line_style::allow_long_disguise).extra_style_parser(&allow_negative_numbers).run(), vm);

        if (vm.count("help")) {
            std::cout << opt_exec << std::endl;
            std::exit(1);
        }
        po::notify(vm);

        YAML::Node yaml_data = YAML::LoadFile(vm["input"].as<std::string>());

       
        log_sub_cmd_end("info");
        
    } else {
        std::cout << "The specified subcommand is not defined!\n";
    }
    
    //
    return 0;
}


