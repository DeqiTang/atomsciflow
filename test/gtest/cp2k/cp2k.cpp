// @file cp2k.cpp
// @author: DeqiTang
// Mail: deqi_tang@163.com 
// Created Time: Wed 17 Nov 2021 11:59:31 AM CST

#include <gtest/gtest.h>
#include <iostream>

#include "atomsciflow/cp2k/cp2k.h"

TEST(cp2k, section) {
    //EXPECT_EQ(atomsciflow::version(), "0.0.0");
    auto cp2k = new atomsciflow::Cp2k;
    
    //std::cout << cp2k->to_string() << std::endl;

    EXPECT_EQ(cp2k->sections["global"].get<std::string>("project"), "cp2k_job");
    cp2k->sections["global"].set_param("project", "cp2k_test");
    EXPECT_EQ(cp2k->sections["global"].get<std::string>("project"), "cp2k_test");

    
    cp2k->sections["global"].add_subsection("test");
    cp2k->sections["global"].subsections["test"].set_param("test", std::vector<int>{3, 2, 1});
    auto out = cp2k->sections["global"].subsections["test"].get<std::vector<int> >("test");
    auto compare = std::vector<int>{3, 2, 1};
    EXPECT_EQ(out, compare);

    delete cp2k;
}
