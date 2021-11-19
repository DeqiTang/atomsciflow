// @file section_v2.h
// @author: DeqiTang
// Mail: deqi_tang@163.com 
// Created Time: Wed 17 Nov 2021 10:40:38 PM CST

// QmcpackSectionV1 make use of VariableGroupV1 to keep the variables and
// use boost::property_tree::ptree to do the xml related parsing and output
// This is unlike QmcpackSectionV2 which only use boost;;property_tree::ptree to
// both store the variables and do the xml related parsing.


#ifndef ATOMSCIFLOW_QUMCPACK_SECTION_V2_h_
#define ATOMSCIFLOW_QUMCPACK_SECTION_V2_h_

#include <memory>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>


namespace atomsciflow {

namespace bpt = boost::property_tree;

class QmcpackSectionV2 {

public:

    QmcpackSectionV2() {
        this->pt = std::make_shared<bpt::ptree>();
    };
    
    ~QmcpackSectionV2() {
    };


    std::shared_ptr<bpt::ptree> pt; 

};

} // naemsapce atomsciflow

#endif // ATOMSCIFLOW_QUMCPACK_SECTION_V2_h_

