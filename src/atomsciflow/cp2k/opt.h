// @file opt.h
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Sun 23 Jan 2022 03:12:04 PM CST

#ifndef ATOMSCIFLOW_CP2K_OPT_H_
#define ATOMSCIFLOW_CP2K_OPT_H_

#include "atomsciflow/cp2k/cp2k.h"


namespace atomsciflow {


class OptRun : public Cp2k {

public:
    
    OptRun() {
        this->sections["force_eval"].subsections["mgrid"].set_param("cutoff", 100);
        this->sections["force_eval"].subsections["mgrid"].set_param("rel_cutoff", 60);
    }

    void set_geo_opt();
    void geo_opt(std::string directory, std::string inpname, std::string output, std::string runopt, int auto_level);

};


} // namespace atomsciflow
#endif // ATOMSCIFLOW_CP2K_OPT_H_

