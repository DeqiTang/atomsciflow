/************************************************************************
    > File Name: group.h
    > Author: deqi
    > Mail: deqi_tang@163.com 
    > Created Time: Sat 30 Jan 2021 08:14:26 PM CST
************************************************************************/

#ifndef ATOMSCIFLOW_VARIABLE_GROUP_V2_H_
#define ATOMSCIFLOW_VARIABLE_GROUP_V2_H_

#include <map>
#include <iostream>
#include <iterator>
#include <typeinfo>

//#include "atomsciflow/abinit/utils.h"
#include "atomsciflow/variable/variable_v2.h"

namespace atomsciflow {


class VariableGroupV2 {// }: public AbinitVariableGroupBase {
    public:
        VariableGroupV2() {};
        virtual ~VariableGroupV2() {};

        virtual void set_param(std::string key, int value);
        virtual void set_param(std::string key, double value);
        virtual void set_param(std::string key, std::vector<int> value);
        virtual void set_param(std::string key, std::vector<double> value);

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
        std::map<std::string, VariableV2<int> > params_int;
        std::map<std::string, VariableV2<double> > params_double;

};


} // namespace atomsciflow

#endif // ATOMSCIFLOW_VARIABLE_GROUP_V2_H_

