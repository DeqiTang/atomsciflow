// @file cmd_utils.cpp
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Sun 23 Jan 2022 11:15:36 PM CST

#include "cmd_utils.h"

namespace po = boost::program_options;

namespace filesys = std::filesystem; 
//namespace filesys = boost::filesystem;     // --std=c++11 -lboost_filesystem -lboost_system

int log_sub_cmd_start(std::string cmd) {
    std::cout << "----------------------------------------------------------------------" << std::endl;    
    std::cout << "sub commands -> " << cmd << std::endl;
    std::cout << "Run info:" << std::endl;
    return 0;
}

int log_sub_cmd_end(std::string cmd) {
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << "sub command: " << cmd << " finished!!!                                      " << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    return 0;
}

// used to allow negative number parsed to boost cmd option
std::vector<po::option> allow_negative_numbers(std::vector<std::string>& args) {
    // this function can help to alow negative number args but it probhibits positional args
    // however we do not need positional args. so it is ok.
    std::vector<po::option> result;
    int pos = 0;
    while(!args.empty()) {
        const auto& arg = args[0];
        double num;
        if(boost::conversion::try_lexical_convert(arg, num)) {
            result.push_back(po::option());
            po::option& opt = result.back();

            opt.position_key = pos++;
            opt.value.push_back(arg);
            opt.original_tokens.push_back(arg);

            args.erase(args.begin());
        } else {
            break;
        }
    }

    return result;
}


