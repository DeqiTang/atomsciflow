// @file src/atomsciflow/parser/json.cpp
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Sun 13 Feb 2022 03:57:49 PM CST

#include "atomsciflow/parser/json.h"

namespace atomsciflow {

void Json::read_json(const std::string& path) {

    boost::property_tree::read_json(path, this->pt);

}


} // namespace atomsciflow
