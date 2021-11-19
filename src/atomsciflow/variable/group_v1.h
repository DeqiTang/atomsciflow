/************************************************************************
    > File Name: group_v1.h
    > Author: deqi
    > Mail: deqi_tang@163.com 
    > Created Time: Sat 30 Jan 2021 08:14:26 PM CST
************************************************************************/

#ifndef ATOMSCIFLOW_VARIABLE_GROUP_V1_H_
#define ATOMSCIFLOW_VARIABLE_GROUP_V1_H_

#include <map>
#include <iostream>
#include <iterator>
#include <typeinfo>

//#include "atomsciflow/variable/utils.h"
#include "atomsciflow/variable/variable_v1.h"

namespace atomsciflow {

class VariableGroupV1 {//}: public AbinitVariableGroupBase {
    public:
        VariableGroupV1() {};
        virtual ~VariableGroupV1() {};

        virtual void set_param(std::string key, int value);
        virtual void set_param(std::string key, double value);
        virtual void set_param(std::string key, std::string value);
        virtual void set_param(std::string key, std::vector<int> value);
        virtual void set_param(std::string key, std::vector<double> value);
        virtual void set_param(std::string key, std::vector<std::string> value);

        virtual void set_param(std::string key, std::vector<std::vector<int>> value);
        virtual void set_param(std::string key, std::vector<std::vector<double>> value);
        virtual void set_param(std::string key, std::vector<std::vector<std::string>> value);

        virtual std::string to_string();
        virtual std::string to_string(int n);

        virtual bool contains(std::string key);
        virtual void set_status(std::string key, bool status);
        virtual void remove(std::string key);

        virtual void clear();

        template<typename U>
        U get(std::string key);

        int n;
    private:
        std::map<std::string, VariableV1> params;
};


} // namespace atomsciflow

#endif // ATOMSCIFLOW_VARIABLE_GROUP_V1_H_

