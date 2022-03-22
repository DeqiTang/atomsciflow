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


#include "atomsciflow_post_vasp.h"

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
#include "atomsciflow/base/kpath.h"
#include "atomsciflow/utils.h"
#include "atomsciflow/vasp/post/phonopy.h"

// needs: libboost-dev, libboost-program-options-dev

namespace po = boost::program_options;


//namespace filesys = std::filesystem; 
namespace filesys = boost::filesystem;     // --std=c++11 -lboost_filesystem -lboost_system
    

void atomsciflow_post_vasp(po::parsed_options& parsed, po::variables_map& vm) {

    // convert command has the following options:
    po::options_description opt_vasp_post("vasp post options");
    opt_vasp_post.add_options()
        ("help, h", "Print out help information for vasp post sub command")
        ("runtype, r", po::value<int>()->default_value(0), "choices of runtype. 0->static_run; 1->optimization; 2->cubic-cell; 3->hexagonal-cell; 4->tetragonal-cell; 5->neb; 6->vasp-phonon; 7->phonopy; 10-md")
        ("static", po::value<std::string>()->default_value("band")->required(), "in case of band(default), run scf, nscf(bands) in a single run; in case of scf, run scf only, in case of optics, run scf and optics calc in a single run")
        ("directory, d", po::value<std::string>()->default_value("askit-calc-running")->required(), "Directory for the running.")
        ("xyz", po::value<std::string>(), "The extended xyz structure file")
        ("cif", po::value<std::string>(), "The cif structure file")
        ("kpath-manual", po::value<std::vector<std::string>>(), "manual input kpath for band structure calculation")
        ("kpath-file", po::value<std::string>()->default_value(""), "file to read the kpath for band structure calculation")
        ("supercell-n", po::value<std::vector<int>>(), "supercell for phonopy, like [2, 2, 2]")
        ("tmax", po::value<int>()->default_value(1500), "phonopy thermo properties calc temperature max")
    ;
    std::vector<std::string> opts = po::collect_unrecognized(parsed.options, po::include_positional);
    opts.erase(opts.begin());
    //parse again...
    po::store(po::command_line_parser(opts).options(opt_vasp_post).style(po::command_line_style::unix_style | po::command_line_style::allow_long_disguise).extra_style_parser(&allow_negative_numbers).run(), vm);

    if (vm.count("help")) {
        std::cout << opt_vasp_post << std::endl;
        std::exit(1);
    }
    po::notify(vm);

    std::string xyzfile;
    if (vm.count("xyz")) {
        xyzfile = vm["xyz"].as<std::string>();
    } else if (vm.count("cif")) {
        //
    }

    if (vm["runtype"].as<int>() == 7) {
        auto task = atomsciflow::PhonopyPost();
        task.supercell_n = vm["supercell-n"].as<std::vector<int>>();
        task.get_kpath(atomsciflow::get_kpath(vm["kpath-manual"].as<std::vector<std::string>>(), vm["kpath-file"].as<std::string>()));
        task.get_xyz(xyzfile);
        task.process(vm["directory"].as<std::string>(), vm["tmux"].as<int>());
    }

} 


