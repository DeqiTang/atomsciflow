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

// needs: libboost-dev, libboost-program-options-dev

namespace po = boost::program_options;


//namespace filesys = std::filesystem; 
namespace filesys = boost::filesystem;     // --std=c++11 -lboost_filesystem -lboost_system
    

void atomsciflow_calc_cp2k(po::parsed_options& parsed, po::variables_map& vm) {

    // convert command has the following options:
    po::options_description opt_cp2k("cp2k options");
    opt_cp2k.add_options()
        ("help, h", "Print out help information for cp2k sub command")
        ("runtype, r", po::value<int>()->default_value(0), "Choice of the calculation type: 0 -> static run, 1 -> geometric optimization")
        ("input, i", po::value<std::string>()->required(), "input structure file")
        ("directory, d", po::value<std::string>()->default_value("askit-calc-running"), "The directory to put all the resources")
        ("runopt", po::value<std::string>()->default_value("gen"), "Rnning option, generation only, or running at the same time")
        ("auto", po::value<int>()->default_value(3), "Automation level: 0 -> doing nothing, 1 -> copying files to server")
        ("mpi", po::value<std::string>()->default_value(""))
        ("server", po::value<std::string>()->default_value("pbs"))
        ("jobname", po::value<std::string>()->default_value("atomsciflow-job"))
        ("nodes", po::value<int>()->default_value(1))
        ("ppn", po::value<int>()->default_value(32))
        // llhpc
        ("parition", po::value<std::string>()->default_value("free"))
        ("ntask", po::value<int>()->default_value(24))
        ("stdout", po::value<std::string>()->default_value("slurm.out"))
        ("stderr", po::value<std::string>()->default_value("slurm.err"))
    ;
    std::vector<std::string> opts = po::collect_unrecognized(parsed.options, po::include_positional);
    opts.erase(opts.begin());
    //parse again...
    po::store(po::command_line_parser(opts).options(opt_cp2k).style(po::command_line_style::unix_style | po::command_line_style::allow_long_disguise).extra_style_parser(&allow_negative_numbers).run(), vm);

    if (vm.count("help")) {
        std::cout << opt_cp2k << std::endl;
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
        // std::cout << "in_path(extension)->" << in_path.extension().string() << std::endl;
        crystal = atomsciflow::read_structure_file(input_file); 

        //atomsciflow::Cp2k calculator;
        //calculator.set_subsys(crystal);
        //std::cout << calculator.to_string() << std::endl;
    
        if (vm["runtype"].as<int>() == 1) {

            auto task = atomsciflow::Cp2kOpt();
            task.set_subsys(crystal);
            task.geo_opt(
                vm["directory"].as<std::string>(),
                "geo-opt.in", 
                "geo-opt.out", 
                vm["runopt"].as<std::string>(), 
                vm["auto"].as<int>()
            );
        }

    }

}  
