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

/// @file src/atomsciflow/cmd/cmd_utils.h
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Sun 23 Jan 2022 11:13:48 PM CST

#ifndef ATOMSCIFLOW_CMD_CMD_UTILS_H
#define ATOMSCIFLOW_CMD_CMD_UTILS_H

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <regex>
#include <cstdlib>

#include "atomsciflow/base/kpath.h"

int log_cmd_start(const std::string& cmd);
int log_sub_cmd_start(const std::string& cmd);
int log_sub_cmd_end(const std::string& cmd);

// used to allow negative number parsed to boost cmd option
std::vector<boost::program_options::option> allow_negative_numbers(std::vector<std::string>& args);

#endif // ATOMSCIFLOW_CMD_CMD_UTILS_H
