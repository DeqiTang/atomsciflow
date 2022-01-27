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
#include "atomsciflow/qe/pw.h"

#include "atomsciflow_calc_abinit.h"
#include "atomsciflow_calc_cp2k.h"
#include "atomsciflow_calc_qe.h"
#include "cmd_utils.h"

// needs: libboost-dev, libboost-program-options-dev
namespace po = boost::program_options;


//namespace filesys = std::filesystem; 
namespace filesys = boost::filesystem;     // --std=c++11 -lboost_filesystem -lboost_system


int main(int argc, char const* argv[]) {
    //
    po::options_description global("Global options");
    global.add_options()
        ("command", po::value<std::string>(), "command to execute")
        ("subargs", po::value<std::vector<std::string> >(), "Arguments for command")
        ("help, h", "print out help information");
        
    po::positional_options_description pos;
    pos.add("command", 1).add("subargs", -1);
    
    po::variables_map vm;
    
    po::parsed_options parsed = po::command_line_parser(argc, argv).
        options(global).
        positional(pos).
        allow_unregistered().
        run();
        
    po::store(parsed, vm);

    std::cout << "**********************************************************************" << std::endl;
    std::cout << "*                     atomsciflow-calc.x utils runnig                     *" << std::endl;
    std::cout << "**********************************************************************" << std::endl;
    
    if (0 == vm.count("command")) { // or by vm.empty()
        std::cout << "You didn't specify any subcommand!\n";
        std::exit(1);
    }
    std::string cmd = vm["command"].as<std::string>();
    

    if (cmd == "abinit") {
        //
        log_sub_cmd_start(cmd);
        
        atomsciflow_calc_abinit(parsed, vm);
        
        log_sub_cmd_end(cmd);
        
    } else if (cmd == "cp2k") {
    
        log_sub_cmd_start(cmd);
        
        atomsciflow_calc_cp2k(parsed, vm); 

        log_sub_cmd_end(cmd); 

    } else if ("qe" == cmd) {
        //
        log_sub_cmd_start(cmd);
        
        atomsciflow_calc_qe(parsed, vm);        

        log_sub_cmd_end(cmd);    

    } else {
        std::cout << "The specified subcommand is not defined!\n";
    }
        
    return 0;
}
