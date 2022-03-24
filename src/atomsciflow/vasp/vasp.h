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

/// @file src/atomsciflow/vasp/vasp.h
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Fri 04 Mar 2022 04:07:04 PM CST

#ifndef ATOMSCIFLOW_VASP_VASP_H_
#define ATOMSCIFLOW_VASP_VASP_H_

#include <map>

#include "atomsciflow/vasp/vasp_incar.h"
#include "atomsciflow/vasp/vasp_poscar.h"
#include "atomsciflow/vasp/vasp_kpoints.h"
#include "atomsciflow/vasp/limit.h"
#include "atomsciflow/server/job_scheduler.h"

namespace atomsciflow {

class Vasp {
public:

    Vasp();
    ~Vasp();

    void get_xyz(const std::string& xyzfile);
    void set_params(const std::map<std::string, std::string>& params, const std::string& runtype);
    void set_kpoints(const std::vector<int>& kpoints_mp, const std::string& option, Kpath* kpath);
    void set_job_steps_default();

    virtual void run(const std::string& directory);

    VaspIncar* incar;
    VaspPoscar* poscar;
    VaspKpoints* kpoints;

    VaspLimit limit;
    JobScheduler job;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_VASP_VASP_H_