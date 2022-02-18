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

#include "cmd_utils.h"
#include "atomsciflow/remote/ssh.h"

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
    
    std::cout << "**********************************************************************" << std::endl;
    std::cout << "*                       atomsciflow-ssh.x utils runnig                     *" << std::endl;
    std::cout << "**********************************************************************" << std::endl;
    
    if (0 == vm.count("subcommand")) { // or by vm.empty()
        std::cout << "You didn't specify any subcommand!\n";
        std::exit(1);
    }
    std::string subcommand = vm["subcommand"].as<std::string>();


    if (subcommand == "exec") {
        //
        std::cout << "----------------------------------------------------------------------" << std::endl;    
        std::cout << "sub commands -> exec                                               " << std::endl;
        std::cout << "Run info:" << std::endl;

        
        // exec command has the following options:
        po::options_description opt_exec("exec options");
        opt_exec.add_options()
            ("help, h", "Print out help information for exec subcommand")
            ("ip, i", po::value<std::string>()->required(), "IP address")
            ("cmd, c", po::value<std::string>()->required(), "Commands to execute in remote machine")
            ;
        // collect all the unrecognized options from the first pass. this will include the 
        // (positional) command name so we need to erase that
        std::vector<std::string> opts = po::collect_unrecognized(parsed.options, po::include_positional);
        opts.erase(opts.begin());
        //parse again...
        po::store(po::command_line_parser(opts).options(opt_exec).style(po::command_line_style::unix_style | po::command_line_style::allow_long_disguise).extra_style_parser(&allow_negative_numbers).run(), vm);

        if (vm.count("help")) {
            std::cout << opt_exec << std::endl;
            std::exit(1);
        }
        po::notify(vm);


        if (vm.count("ip") && vm.count("cmd")) {
        
            std::string ip = vm["ip"].as<std::string>();
            std::string cmd = vm["cmd"].as<std::string>();
        
            std::cout << "ip adress: " << ip << std::endl;
            std::cout << "command: " << cmd << std::endl;

            atomsciflow::Ssh ssh;
            ssh.connect(ip, "root", "password");
            ssh.execute(cmd);
        }
        
        
        std::cout << "----------------------------------------------------------------------" << std::endl;
        std::cout << "sub command: exec finished!!!                                      " << std::endl;
        std::cout << "----------------------------------------------------------------------" << std::endl;
        
    } else {
        std::cout << "The specified subcommand is not defined!\n";
    }
    
    //
    return 0;
}
