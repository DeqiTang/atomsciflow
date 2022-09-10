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

/// @file src/atomsciflow/server/submit_script.h
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Mon 07 Mar 2022 10:13:27 PM CST

#ifndef ATOMSCIFLOW_SERVER_SUBMIT_H_
#define ATOMSCIFLOW_SERVER_SUBMIT_H_

#include <string>
#include <map>

namespace atomsciflow {

std::string submit_header_llhpc(std::map<std::string, std::string>& params);
std::string submit_header_yh(std::map<std::string, std::string>& params);
std::string submit_header_pbs(std::map<std::string, std::string>& params);
std::string submit_header_bash(std::map<std::string, std::string>& params);
std::string submit_header_lsf_sz(std::map<std::string, std::string>& params);
std::string submit_header_lsf_sustc(std::map<std::string, std::string>& params);
std::string submit_header_cdcloud(std::map<std::string, std::string>& params);
std::string submit_header_slurm(std::map<std::string, std::string>& params);

} // namespace atomsciflow

#endif // ATOMSCIFLOW_SERVER_SUBMIT_H_
