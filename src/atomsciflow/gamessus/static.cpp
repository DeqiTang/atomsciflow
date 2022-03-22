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

/// @file src/atomsciflow/gamessus/static.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Fri 11 Mar 2022 10:22:22 AM CST

#include "atomsciflow/gamessus/static.h"

namespace atomsciflow {

GamessUSStatic::GamessUSStatic() {
    this->new_group("contrl");
    this->new_group("system");
    this->new_group("basis");
    this->new_group("guess");

    this->set_param("contrl", "scftyp", "UHF");
    this->set_param("contrl", "coord", "unique");
    this->set_param("contrl", "mult", 3);
    this->set_param("contrl", "runtyp", "gradient");
    this->set_param("contrl", "local", "boys");

    this->set_param("system", "timlim", 1);

    this->set_param("basis", "gbasis", "sto");
    this->set_param("basis", "ngauss", 2);

    this->set_param("guess", "guess", "huckel");

    this->set_job_steps_default();

}

GamessUSStatic::~GamessUSStatic() {

}

} // namespace atomsciflow
