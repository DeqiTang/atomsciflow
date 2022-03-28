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

/// @file src/atomsciflow/config/config_manager.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Sat 05 Mar 2022 08:32:18 PM CST

#include "atomsciflow/config/config_manager.h"

#include <cstdlib>

namespace atomsciflow {

ConfigManager::ConfigManager() {

    this->home_dir = this->get_home_dir();
    this->config_dir = this->get_config_dir();

#if defined(__linux__) || defined(__APPLE__)
    if (false == fs::exists(fs::path(this->home_dir) / ".atomsciflow")) {
        fs::create_directory(fs::path(this->home_dir) / ".atomsciflow");
    }    
#elif defined(_WIN32)
    if (false == fs::exists(fs::path(this->home_dir) / "atomsciflow")) {
        fs::create_directory(fs::path(this->home_dir) / "atomsciflow");
    }    
#endif

}

ConfigManager::~ConfigManager() {

}

std::string ConfigManager::get_home_dir() {
    std::string home_dir;

#if defined(__linux__) || defined(__APPLE__)
    char* home = std::getenv("HOME");
    home_dir = std::string(home);
#elif defined(_WIN32)
    char* home_path = std::getenv("HOMEPATH");
    home_dir = std::string(home_path);
#endif 

    return home_dir;
}

std::string ConfigManager::get_config_dir() {
    std::string config_dir;

#if defined(__linux__) || defined(__APPLE__)
    config_dir = (fs::path(this->home_dir) / ".atomsciflow").string();
#elif defined(_WIN32)
    config_dir = (fs::path(this->home_dir) / "atomsciflow").string();
#endif

    return config_dir;
}

bool ConfigManager::get_server_info() {
    if (fs::exists(fs::path(this->config_dir) / "server.json")) {
        pt::read_json((fs::path(this->config_dir) / "server.json").string(), this->server_json);
        return true;
    } else {
        return false;
    }
}

std::map<std::string, std::string> ConfigManager::get_pseudo_pot_dir() {
    std::map<std::string, std::string> out;
    if (fs::exists(fs::path(config_dir) / "pseudo-pot.json")) {
        pt::read_json((fs::path(config_dir) / "pseudo-pot.json").string(), this->pseudo_pot_json);
    } else {
        this->pseudo_pot_json.put("abinit.pseudodir", 
            (fs::path(this->get_home_dir()) / ".pseudo-pot-abinit").string());
        this->pseudo_pot_json.put("qe.pseudodir", 
            (fs::path(this->get_home_dir()) / ".pseudo-pot-qe").string());
        this->pseudo_pot_json.put("siesta.pseudodir", 
            (fs::path(this->get_home_dir()) / ".pseudo-pot-siesta").string());
        this->pseudo_pot_json.put("vasp.pseudodir",
            (fs::path(this->get_home_dir()) / ".pseudo-pot-vasp").string());
        pt::write_json((fs::path(config_dir) / "pseudo-pot.json").string(), this->pseudo_pot_json);
    }
    out["abinit"] = this->pseudo_pot_json.get<std::string>("abinit.pseudodir");
    out["qe"] = this->pseudo_pot_json.get<std::string>("qe.pseudodir");
    out["siesta"] = this->pseudo_pot_json.get<std::string>("siesta.pseudodir");
    out["vasp"] = this->pseudo_pot_json.get<std::string>("vasp.pseudodir");
    return out;
}

} // namespace atomsciflow
