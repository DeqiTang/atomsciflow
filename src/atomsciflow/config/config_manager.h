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

/// @file src/atomsciflow/config/config_manager.h
/// @author: DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Sat 05 Mar 2022 08:30:36 PM CST

#ifndef ATOMSCIFLOW_CONFIG_CONFIG_MANAGERE_H_
#define ATOMSCIFLOW_CONFIG_CONFIG_MANAGERE_H_

#include <map>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace atomsciflow {

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

class ConfigManager {

public:
    ConfigManager();
    ~ConfigManager();

    std::string get_home_dir();
    std::string get_config_dir();
    bool get_server_info();
    std::map<std::string, std::string> get_pseudo_pot_dir();

    std::string home_dir;
    std::string config_dir;
    pt::ptree server_json;
    pt::ptree pseudo_pot_json;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_CONFIG_CONFIG_MANAGERE_H_
