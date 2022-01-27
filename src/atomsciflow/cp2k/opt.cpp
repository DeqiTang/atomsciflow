// @file opt.cpp
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Sun 23 Jan 2022 03:20:37 PM CST

#include "atomsciflow/cp2k/opt.h"

//#include <filesystem>
#include <boost/filesystem.hpp>
#include <fstream>


namespace atomsciflow {

//namespace fs = std::filesystem;
namespace fs = boost::filesystem;    // --std=c++11 -lboost_filesystem -lboost_system

void OptRun::set_geo_opt() {
    this->sections["global"].set_param("run_type", "GEO_OPT");
    this->sections["motion"].set_status("geo_opt", true);
}

void OptRun::geo_opt(std::string directory = "tmp-cp2k-geo-opt", std::string inpname = "geo-opt.inp", std::string output = "geo-opt.out", std::string runopt = "gen", int auto_level = 0) {
    this->set_geo_opt();
    
    if (runopt == "gen" || runopt == "genrun") {
        if (fs::exists(fs::path(directory))) {
            fs::remove_all(fs::path(directory));
        }
        fs::create_directory(fs::path(directory));
        //fs::copy()

        std::ofstream out;
        auto inp_path = fs::path(directory);
        inp_path /= inpname;
        out.open(inp_path.string());
        out << this->to_string();

        this->gen_cdcloud(inpname, output, directory, "$ASF_CP2K");
    }

    if (runopt =="run" || runopt == "genrun") {
       
        
    }

}



} // namespace atomsciflow
