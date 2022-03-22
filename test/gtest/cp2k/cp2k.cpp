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
