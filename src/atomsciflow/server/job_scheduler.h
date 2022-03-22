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

/// @file src/atomsciflow/server/job_scheduler.h
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Thu 17 Mar 2022 08:21:39 PM CST

#ifndef ATOMSCIFLOW_SERVER_JOB_SCHEDULER_H_
#define ATOMSCIFLOW_SERVER_JOB_SCHEDULER_H_

#include <string>
#include <map>
#include <vector>

namespace atomsciflow {

class JobScheduler {
public:
    JobScheduler();
    ~JobScheduler() = default;

    void set_run(std::string key, std::string value);
    void set_run_default(std::string platform);
    void set_run_default_llhpc();
    void set_run_default_yh();
    void set_run_default_pbs();
    void set_run_default_bash();
    void set_run_default_lsf_sz();
    void set_run_default_lsf_sustc();
    void set_run_default_cdcloud();
    void gen_llhpc(const std::string& script);
    void gen_yh(const std::string& script);
    void gen_pbs(const std::string& script);
    void gen_bash(const std::string& script);
    void gen_lsf_sz(const std::string& script);
    void gen_lsf_sustc(const std::string& script);
    void gen_cdcloud(const std::string& script);

    void run(const std::string& directory);

    std::map<std::string, std::string> run_params;
    std::vector<std::string> steps;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_SERVER_JOB_SCHEDULER_H_
