// @file phonopy.cpp
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Mon 28 Feb 2022 11:06:10 PM CST

#include "atomsciflow/vasp/post/phonopy.h"

#include <iostream>

namespace atomsciflow {


PhonopyPost::PhonopyPost() {
    YAML::Node yaml_node = YAML::Load("[1, 2, 3, 4, 5, 6]");
    int size = yaml_node.size();
    //for (int i = 0; i < size; i++) {
    //    std::cout << yaml_node[i].as<int>() << std::endl;
    //}
}

PhonopyPost::~PhonopyPost() {

}

} // namespace atomsciflow
