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

/// @file src/atomsciflow/remote/server.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Tue 08 Mar 2022 02:14:37 PM CST

#include "atomsciflow/remote/server.h"

#include <boost/filesystem.hpp>

namespace atomsciflow {

namespace fs = boost::filesystem;

/**
 * @param auto
 *      0: doing nothing for the call to server_handle
 *      1: copying files to server
 *      2: copying file to server and executing the job
 *      3: running in server, direct submit the job
 *      in order use auto = 1, 2, you must make sure there is a working ~/.atomsciflow/server_[pbs|yh].conf"
 * @param server
 *  candidates:
 *      'pbs' | 'llhpc' | 'lsf_sz' | 'lsf_sustc
 * @param jobfilebase base name of submitting job script,
 *      like static-nscf and the coresponding job submit script
 *      would be static-nscf.pbs if server is pbs and sstatic-nscf.yh
 *      if server is llhpc
 */
void server_handle(std::string directory, std::string jobfilebase, std::string server, int auto_level) {
    if (0 == auto_level) {

    } else if (1 == auto_level) {

    } else if (2 == auto_level) {

    } else if (3 == auto_level) {
        std::string cmd = "";
        if ("pbs" == server) {
            cmd += "qsub ";
            cmd += (fs::path(directory) / jobfilebase).string() + ".pbs";
            std::system(cmd.c_str());
        } else if ("llhpc" == server) {
            cmd += "yhbatch ";
            cmd += (fs::path(directory) / jobfilebase).string() + ".slurm";
            std::system(cmd.c_str());
        } else if ("lsf_sz" == server) {
            cmd += "chmod 755 ";
            cmd += (fs::path(directory) / jobfilebase).string() + ".lsf_sz";
            cmd += "; bsub ";
            cmd += (fs::path(directory) / jobfilebase).string() + ".lsf_sz";
            std::system(cmd.c_str());
        } else if ("lsf_sustc" == server) {
            cmd += "chmod 755 ";
            cmd += (fs::path(directory) / jobfilebase).string() + ".lsf_sustc";
            cmd += "; bsub ";
            cmd += (fs::path(directory) / jobfilebase).string() + ".lsf_sustc";
            std::system(cmd.c_str());
        } else if ("cdcloud" == server) {
            cmd += "sbatch < ";
            cmd += (fs::path(directory) / jobfilebase).string() + ".slurm_cd";
            std::system(cmd.c_str());
        }
    }
}

} // namespace atomsciflow
