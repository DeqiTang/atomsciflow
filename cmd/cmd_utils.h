// @file cmd_utils.h
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Sun 23 Jan 2022 11:13:48 PM CST

#ifndef CMD_UTILS_H
#define CMD_UTILS_H

#include <boost/program_options.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <regex>
#include <cstdlib>


int log_sub_cmd_start(std::string cmd);

int log_sub_cmd_end(std::string cmd);

// used to allow negative number parsed to boost cmd option
std::vector<boost::program_options::option> allow_negative_numbers(std::vector<std::string>& args);


#endif // CMD_UTILS_H
