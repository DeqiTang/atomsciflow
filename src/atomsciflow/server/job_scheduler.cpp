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

/// @file src/atomsciflow/server/job_scheduler.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Thu 17 Mar 2022 08:22:23 PM CST

#include "atomsciflow/server/job_scheduler.h"

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

#include "atomsciflow/server/submit_script.h"
#include "atomsciflow/remote/server.h"
#include "atomsciflow/server/utils.h"

namespace atomsciflow {

namespace fs = boost::filesystem;

JobScheduler::JobScheduler() {
    this->set_run_default("bash");
    this->set_run_default("llhpc");
    this->set_run_default("pbs");
    this->set_run_default("bash");
    this->set_run_default("lsf_sz");
    this->set_run_default("lsf_sustc");
    this->set_run_default("cdcloud");
}

void JobScheduler::set_run(std::string key, std::string value) {
    this->run_params[key] = value;
}

/// @param runopt the running options
///  candidates:
///     gen    -> only generate the files for calculation
///     run    -> for previously generated files run them
///     genrun -> generate the needed files and run it
/// @param auto_level
///     0 -> nothing to do
///     1 -> tansferring the files to the server
void JobScheduler::set_run_default(std::string platform) {
    this->set_run("runopt", "genrun");
    this->set_run("auto_level", "0");

    if ("llhpc" == platform) {
        this->set_run_default_llhpc();
    } else if ("yh" == platform) {
        this->set_run_default_yh();
    } else if ("pbs" == platform) {
        this->set_run_default_pbs();
    } else if ("bash" == platform) {
        this->set_run_default_pbs();
    } else if ("lsf_sz" == platform) {
        this->set_run_default_lsf_sz();
    } else if ("lsf_sustc" == platform) {
        this->set_run_default_lsf_sustc();
    } else if ("cdcloud" == platform) {
        this->set_run_default_cdcloud();
    }
}

void JobScheduler::set_run_default_llhpc() {
    this->run_params["partition"] = "free";
    this->run_params["jobname"] = "running";
    this->run_params["nodes"] = std::to_string(1);
    this->run_params["ntask"] = boost::lexical_cast<std::string>(24);
    this->run_params["stdout"] = "slurm.out";
    this->run_params["stderr"] = "slurm.err";
}

void JobScheduler::set_run_default_yh() {
}

void JobScheduler::set_run_default_pbs() {
    this->run_params["nodes"] = boost::lexical_cast<std::string>(1);
    this->run_params["ppn"] = boost::lexical_cast<std::string>(32);
}

void JobScheduler::set_run_default_bash() {
    this->run_params["cmd"] = "$ELK_BIN";
    this->run_params["mpi"] = "";
}

void JobScheduler::set_run_default_lsf_sz() {
    this->run_params["np"] = "24";
    this->run_params["np_per_node"] = "12";
    this->run_params["queue"] = "intelY_mid";
}

void JobScheduler::set_run_default_lsf_sustc() {
    this->run_params["jobname"] = "running-job";
    this->run_params["np" ] = "24";
    this->run_params["np_per_node"] = "12";
    this->run_params["queue"] = "medium";
}

void JobScheduler::set_run_default_cdcloud() {
    this->run_params["partition"] = "normal";
    this->run_params["nodes"] = "1";
    this->run_params["ntask"] = "32";
    this->run_params["jobname"] = "running-job";
    this->run_params["stdout"] = "slurm.out";
    this->run_params["stderr"] = "slurm.err";
}

void JobScheduler::gen_llhpc(const std::string& script = "vasp.sub") {

    std::ofstream outfile;
    outfile.open(script);
    outfile << "";
    outfile.setf(std::ios::fixed);

    outfile << submit_header_llhpc(this->run_params);
    outfile << "cd ${ABSOLUTE_WORK_DIR}" << "\n";

    int i = 0;
    for (const auto& step : this->steps) {
        outfile << script_step_header(i);
        outfile << step << "\n";
        i++;
    }

    outfile.close();
}

void JobScheduler::gen_yh(const std::string& script) {
    std::ofstream outfile;
    outfile.open(script);
    outfile.setf(std::ios::fixed);

    outfile << submit_header_yh(this->run_params);
    outfile << "cd ${ABSOLUTE_WORK_DIR}" << "\n";

    int i = 0;
    for (const auto& step : this->steps) {
        outfile << script_step_header(i);
        outfile << step << "\n";
        i++;
    }

    outfile.close();
}

void JobScheduler::gen_pbs(const std::string& script) {
    std::ofstream outfile;
    outfile.open(script);

    outfile << submit_header_pbs(this->run_params);
    outfile << "cd ${ABSOLUTE_WORK_DIR}" << "\n";

    int i = 0;
    for (const auto& step : this->steps) {
        outfile << script_step_header(i);
        outfile << step << "\n";
        i++;
    }

    outfile.close();
}

void JobScheduler::gen_bash(const std::string& script) {
    std::ofstream outfile;
    outfile.open(script);

    outfile << submit_header_bash(this->run_params);
    outfile << "cd ${ABSOLUTE_WORK_DIR}" << "\n";

    int i = 0;
    for (const auto& step : this->steps) {
        outfile << script_step_header(i);
        outfile << step << "\n";
        i++;
    }

    outfile.close();
}

void JobScheduler::gen_lsf_sz(const std::string& script) {
    std::ofstream outfile;
    outfile.open(script);

    outfile << submit_header_lsf_sz(this->run_params);
    outfile << "cd ${ABSOLUTE_WORK_DIR}" << "\n";

    int i = 0;
    for (const auto& step : this->steps) {
        outfile << script_step_header(i);
        outfile << step << "\n";
        i++;
    }

    outfile.close();
}

void JobScheduler::gen_lsf_sustc(const std::string& script = "vasp.lsf_sustc") {
    std::ofstream outfile;
    outfile.open(script);
    outfile.setf(std::ios::fixed);

    outfile << submit_header_lsf_sustc(this->run_params);
    outfile << "cd ${ABSOLUTE_WORK_DIR}" << "\n";

    int i = 0;
    for (const auto& step : this->steps) {
        outfile << script_step_header(i);
        outfile << step << "\n";
        i++;
    }

    outfile.close();
}

void JobScheduler::gen_cdcloud(const std::string& script = "vasp.sub") {
    std::ofstream outfile;
    outfile.open(script);
    outfile.setf(std::ios::fixed);

    outfile << submit_header_cdcloud(this->run_params);

    int i = 0;
    for (const auto& step : this->steps) {
        outfile << script_step_header(i);
        outfile << step << "\n";
        i++;
    }

    outfile.close();
}

void JobScheduler::run(const std::string& directory) {
    this->set_run("absolute_work_dir", fs::absolute(fs::path(directory)).string());

    if ("gen" == run_params["runopt"] || "genrun" == run_params["runopt"]) {
        if (fs::exists(directory)) {
            fs::remove_all(directory);
        }
        fs::create_directory(directory);
        fs::copy(run_params["xyz_file"], fs::path(directory));

        gen_llhpc((
            fs::path(directory) / (run_params["script_name_head"] + ".slurm")
            ).string()
        );
        gen_bash((
            fs::path(directory) / (run_params["script_name_head"] + ".sh")
            ).string()
        );
        gen_lsf_sz((
            fs::path(directory) / (run_params["script_name_head"] + ".lsf_sz")
            ).string()
        );
        gen_pbs((
            fs::path(directory) / (run_params["script_name_head"] + ".pbs")
            ).string()
        );
        gen_lsf_sustc((
            fs::path(directory) / (run_params["script_name_head"] + ".lsf_sustc")
            ).string()
        );
        gen_cdcloud((
            fs::path(directory) / (run_params["script_name_head"] + ".slurm_cd")
            ).string()
        );
    }
    if ("run" == run_params["runopt"] || "genrun" == run_params["runopt"]) {
        std::string cmd = "";
        cmd += "bash ";
        cmd += (
            fs::path(directory) /
            (run_params["script_name_head"] + ".sh")
        ).string();
        std::system(cmd.c_str());
    }
    server_handle(
        directory,
        run_params["script_name_head"],
        run_params["server"],
        boost::lexical_cast<int>(run_params["auto_level"])
    );
}

} // namespace atomsciflow
