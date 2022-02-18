// @file src/atomsciflow/parser/json.h
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Sun 13 Feb 2022 12:47:15 AM CST

#ifndef ATOMSCIFLOW_PARSER_JSON_H_
#define ATOMSCIFLOW_PARSER_JSON_H_

//#include <boost/json.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace atomsciflow {

class Json {

public:
    
    void read_json(const std::string& path);

    boost::property_tree::ptree pt;

};



} // namespace atomsciflow

#endif // ATOMSCIFLOW_PARSER_JSON_H_
