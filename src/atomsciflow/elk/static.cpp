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

/// @file src/atomsciflow/elk/static.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Fri 11 Mar 2022 04:18:41 PM CST

#include "atomsciflow/elk/static.h"

namespace atomsciflow {

ElkStatic::ElkStatic() {
    this->new_block("tasks");
    this->new_block("avec");
    this->new_block("scale1");
    this->new_block("scale2");
    this->new_block("scale3");
    this->new_block("scale");
    this->new_block("lmaxapw");
    this->new_block("atoms");

    this->blocks["tasks"]->set_block_value(0);
    this->blocks["scale1"]->set_block_value(1.0);
}

ElkStatic::~ElkStatic() {

}

} // namespace atomsciflow
