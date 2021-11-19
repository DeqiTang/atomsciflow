/************************************************************************
    > File Name: section_v1.h
    > Author: deqi
    > Mail: deqi_tang@163.com 
************************************************************************/


// QmcpackSectionV1 make use of VariableGroupV1 to keep the variables and
// use boost::property_tree::ptree to do the xml related parsing and output
// This is unlike QmcpackSectionV2 which only use boost;;property_tree::ptree to
// both store the variables and do the xml related parsing.


#ifndef ATOMSCIFLOW_QMCPACK_SECTION_V1_H_
#define ATOMSCIFLOW_QMCPACK_SECTION_V1_H_

#include <map>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "atomsciflow/variable/variable_v1.h"


namespace atomsciflow {

namespace bpt = boost::property_tree;

class QmcpackSectionV1 {

public:
    QmcpackSectionV1() {};
    explicit QmcpackSectionV1(std::string name) { this->name = name; }
    ~QmcpackSectionV1() {};


    std::string to_string();
    std::string to_string(std::string indent);

    //void add_subsection(std::string);
    //void add_subsection(std::string, QmcpackSectionV1);
    QmcpackSectionV1& add_subsection(std::string);
    QmcpackSectionV1& add_subsection(std::string, QmcpackSectionV1);

    void remove_subsection(std::string);

    void set_param(std::string key, int value);
    void set_param(std::string key, double value);
    void set_param(std::string key, std::string value);
    void set_param(std::string key, std::vector<int> value);
    void set_param(std::string key, std::vector<double> value);
    void set_param(std::string key, std::vector<std::string> value);

    void set_param(std::string key, std::vector<std::vector<int>> value);
    void set_param(std::string key, std::vector<std::vector<double>> value);
    void set_param(std::string key, std::vector<std::vector<std::string>> value);

    bool contains(std::string key);
    void set_status(std::string key, bool status);
    void remove(std::string key);

    void clear();

    template<typename U>
    U get(std::string key);

    std::string name = "unknown";
    std::string section_parameter;
    VariableV1 section_var;
    
    bool status = true;

    private:
    std::map<std::string, VariableV1> params;
    std::map<std::string, QmcpackSectionV1> subsections;

    bpt::ptree ptree_obj;

};


} // namespace atomsciflow


#endif // ATOMSCIFLOW_QMCPACK_SECTION_V1_H_

