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
//#include <experimental/filesystem>
#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include <regex>
#include <cstdlib>
#include <armadillo>
#include "atomsciflow/parser/cif.h"
#include "atomsciflow/parser/xyz.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/parser/xyztraj.h"
#include "atomsciflow/utils.h"

#include "atomsciflow/cmd/cmd_utils.h"

namespace po = boost::program_options;

//namespace filesys = std::experimental::filesystem;  // --std=c++17 -lstdc++fs
namespace filesys = boost::filesystem;     // --std=c++11 -lboost_filesystem -lboost_system


int main(int argc, char const* argv[]) {
    //
    po::options_description global("Global options");
    global.add_options()
        ("subcommand", po::value<std::string>(), "subcommand to execute")
        ("subargs", po::value<std::vector<std::string> >(), "Arguments for subcommand")
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
   
    log_cmd_start("atomsciflow-traj.x");
    
    if (0 == vm.count("subcommand")) { // or by vm.empty()
        std::cout << "You didn't specify any subcommand!\n";
        std::exit(1);
    }
    std::string subcommand = vm["subcommand"].as<std::string>();


    if ("tidy" == subcommand) {
        //
        log_sub_cmd_start("tidy");

        // tidy command has the following options:
        po::options_description opt_tidy("tidy options");
        opt_tidy.add_options()
            ("input, i", po::value<std::string>()->required(), "input xyz trajectory file")
            ("output, o", po::value<std::string>()->required(), "output xyz trajectory file")
            ("cell", po::value<std::vector<double>>()->multitoken()->required(), "cell parameter");
        //opts.add_options()
        //    ("input, i", po::value<std::string>(&input), "input structure file")
        //    ("output, o", po::value<std::string>(&output), "output structure file");
        // collect all the unrecognized options from the first pass. this will include the 
        // (positional) command name so we need to erase that
        std::vector<std::string> opts = po::collect_unrecognized(parsed.options, po::include_positional);
        opts.erase(opts.begin());
        //parse again...
        po::store(po::command_line_parser(opts).options(opt_tidy).style(po::command_line_style::unix_style | po::command_line_style::allow_long_disguise).extra_style_parser(&allow_negative_numbers).run(), vm);

        if (vm.count("help")) {
            std::cout << opt_tidy << std::endl;
            std::exit(1);
        }
        po::notify(vm);


        if (vm.count("input") && vm.count("output")) {
            std::vector<atomsciflow::Crystal> trajectory;
            //crystal.read_xyz_file(vm["input"].as<std::string>());
            //crystal.write_cif_file(vm["output"].as<std::string>());
        
        
            std::string input_file = vm["input"].as<std::string>();
            std::string output_file = vm["output"].as<std::string>();
        
            filesys::path in_path(input_file);
            filesys::path out_path(output_file);
        
        
            std::cout << "input: " << input_file << std::endl;
            std::cout << "output: " << output_file << std::endl;
        
            // read structure file
            std::cout << "in_path(extension)->" << in_path.extension().string() << std::endl;

            
            atomsciflow::read_xyztraj_file(trajectory, input_file, 0);

            std::cout << "num of images: " << trajectory.size() << std::endl;

            std::vector<std::vector<double>> cell;
            std::vector<double> arg_cell = vm["cell"].as<std::vector<double>>();
            std::vector<double> vec;
            vec.push_back(arg_cell[0]);
            vec.push_back(arg_cell[1]);
            vec.push_back(arg_cell[2]);
            cell.push_back(vec);
            vec.clear();
            vec.push_back(arg_cell[3]);
            vec.push_back(arg_cell[4]);
            vec.push_back(arg_cell[5]);
            cell.push_back(vec);
            vec.clear();
            vec.push_back(arg_cell[6]);
            vec.push_back(arg_cell[7]);
            vec.push_back(arg_cell[8]);
            cell.push_back(vec);
            vec.clear();
        
            std::cout << "cell processed" << std::endl;

            for (auto& image : trajectory) {
                image.cell = cell;
                set_frac_within_zero_and_one(&image);
            }
            std::cout << "tidied" << std::endl;
            // write structure file
            atomsciflow::write_xyztraj_file(trajectory, output_file, 0);
        }
        
        log_sub_cmd_end("tidy");
        
        
    } else {
        std::cout << "The specified subcommand is not defined!\n";
    }
    
    //
    return 0;
}


