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

/// @file src/atomsciflow/cmd/cmd_utils.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Sun 23 Jan 2022 11:15:36 PM CST

#include "cmd_utils.h"

#include <fstream>
//#include <filesystem>
#include <boost/filesystem.hpp>

namespace po = boost::program_options;
namespace ba = boost::algorithm;

//namespace filesys = std::filesystem; 
namespace filesys = boost::filesystem;     // --std=c++11 -lboost_filesystem -lboost_system

int log_cmd_start(const std::string& cmd) {
    std::cout << "***********************************************************************" << std::endl;
    int front_star = (72 - cmd.size()) / 2 - 2;
    int end_star = 72 - front_star - cmd.size() - 3;
    std::cout << "*";
    while (front_star > 0) {
        front_star -= 1;
        std::cout << " ";
    }
    std::cout << cmd;
    while (end_star > 0) {
        end_star -= 1;
        std::cout << " ";
    }
    std::cout << "*\n";
    std::cout << "***********************************************************************" << std::endl;
    return 0;
}

int log_sub_cmd_start(const std::string& cmd) {
    std::cout << "-----------------------------------------------------------------------" << std::endl;
    std::cout << "sub command -> " << cmd << std::endl;
    std::cout << "Run info:" << std::endl;
    return 0;
}

int log_sub_cmd_end(const std::string& cmd) {
    std::cout << "-----------------------------------------------------------------------" << std::endl;
    std::cout << "sub command: " << cmd << " finished!!!                                      " << std::endl;
    std::cout << "-----------------------------------------------------------------------" << std::endl;
    return 0;
}

// used to allow negative number parsed to boost cmd option
std::vector<po::option> allow_negative_numbers(std::vector<std::string>& args) {
    // This function can help to alow negative number args but it probhibits positional args
    // however we do not need positional args. so it is ok.
    std::vector<po::option> result;
    int pos = 0;
    while (!args.empty()) {
        const auto& arg = args[0];
        double num;
        if (boost::conversion::try_lexical_convert(arg, num)) {
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
