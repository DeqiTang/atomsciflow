// @file qmcpack.cpp
// @author: DeqiTang
// Mail: deqi_tang@163.com 
// Created Time: Wed 17 Nov 2021 11:02:08 PM CST

#include <gtest/gtest.h>

#include "atomsciflow/qmcpack/section_v1.h"
#include "atomsciflow/qmcpack/section_v2.h"

TEST(qmcpack, section_v1) {

}


TEST(qmcpack, section_v2) {

    auto sec = new atomsciflow::QmcpackSectionV2;

    sec->pt->put("project", "qmc_test");

    delete sec;
}



