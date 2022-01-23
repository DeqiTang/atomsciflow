/************************************************************************
    > File Name: cp2k.h
    > Author: deqi
    > Mail: deqi_tang@163.com 
    > Created Time: Sun 31 Jan 2021 07:24:18 PM CST
************************************************************************/
#ifndef ATOMSCIFLOW_CP2K_CP2K_H_
#define ATOMSCIFLOW_CP2K_CP2K_H_

#include <string>
//#include <format> // support for c++20 only

#include "atomsciflow/cp2k/gen_section_v1.h"
#include "atomsciflow/base/crystal.h"

namespace atomsciflow {

class Cp2k {

public:

    Cp2k() {
        this->sections["global"] = cp2k::gen::global();
        this->sections["force_eval"] = cp2k::gen::force_eval();
    };
    ~Cp2k() {};

    std::string to_string() {
        std::string out = "";
        
        for (auto item : this->sections) {
            out += this->sections[item.first].to_string("  ") + "\n";
            out += "\n";
        }
        return out;
    }

    Cp2kSectionV1& set_subsys(Crystal& crystal);

    void set_cdcloud(std::string partition, int nodes, int ntask, std::string jobname, std::string stdout, std::string stderr); 

    std::string gen_cdcloud_string(std::string inpname, std::string output, std::string cmd);
    void gen_cdcloud(std::string inpname, std::string output, std::string directory, std::string cmd);

    std::map<std::string, Cp2kSectionV1> sections;
    Crystal crystal;

    std::map<std::string, std::string> run_params;

private:
    Cp2kSectionV1& set_subsys();

};



} // namespace atomsciflow


#endif // ATOMSCIFLOW_CP2K_CP2K_H_
