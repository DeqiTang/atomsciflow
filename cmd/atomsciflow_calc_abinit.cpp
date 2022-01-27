// @file atomsciflow_calc_abinit.cpp
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Sun 23 Jan 2022 11:58:03 PM CST

#include "atomsciflow_calc_abinit.h"

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

// needs: libboost-dev, libboost-program-options-dev

namespace po = boost::program_options;


//namespace filesys = std::filesystem; 
namespace filesys = boost::filesystem;     // --std=c++11 -lboost_filesystem -lboost_system
    

void atomsciflow_calc_abinit(po::parsed_options& parsed, po::variables_map& vm) {

        // convert command has the following options:
        po::options_description opt_abinit("abinit options");
        opt_abinit.add_options()
            ("help, h", "Print out help information for abinit subcommand")
            ("input, i", po::value<std::string>()->required(), "input structure file");
        //opts.add_options()
        //    ("input, i", po::value<std::string>(&input), "input structure file")
        //    ("output, o", po::value<std::string>(&output), "output structure file");
        // collect all the unrecognized options from the first pass. this will include the 
        // (positional) command name so we need to erase that
        std::vector<std::string> opts = po::collect_unrecognized(parsed.options, po::include_positional);
        opts.erase(opts.begin());
        //parse again...
        po::store(po::command_line_parser(opts).options(opt_abinit).style(po::command_line_style::unix_style | po::command_line_style::allow_long_disguise).extra_style_parser(&allow_negative_numbers).run(), vm);

        if (vm.count("help")) {
            std::cout << opt_abinit << std::endl;
            std::exit(1);
        }
        po::notify(vm);

        if (vm.count("input")) {
            atomsciflow::Crystal crystal;
            //crystal.read_xyz_file(vm["input"].as<std::string>());
            //crystal.write_cif_file(vm["output"].as<std::string>());
        
        
            std::string input_file = vm["input"].as<std::string>();
            //std::string output_file = vm["output"].as<std::string>();
        
            filesys::path in_path(input_file);
            //filesys::path out_path(output_file);
        
        
            std::cout << "input: " << input_file << std::endl;
            //std::cout << "output: " << output_file << std::endl;
        
    
        
            // read structure file
            //std::cout << "in_path(extension)->" << in_path.extension().string() << std::endl;
            crystal = atomsciflow::read_structure_file(input_file);
            
            atomsciflow::Abinit calculator;
            std::cout << calculator.to_string() << std::endl;
        }
}
