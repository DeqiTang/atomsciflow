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

/// @file src/atomsciflow/elk/block.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Fri 11 Mar 2022 04:07:48 PM CST

#include "atomsciflow/elk/block.h"

namespace atomsciflow {

std::string Block::to_string() {
    int n = 0;
    std::string out = "";

    out += this->block_var.to_string(n) + "\n";

    for (const auto& item : this->params) {
        if (this->params[item.first].status == false) {
            continue;
        }
        out += this->params[item.first].to_string(n) + "\n";
    }
    return out;
}

template <typename T>
void Block::set_block_value(T value) {
    this->block_var.set("", value);
}

// explicit template instantiation
template void Block::set_block_value<int>(int);
template void Block::set_block_value<double>(double);
template void Block::set_block_value<std::string>(std::string);
template void Block::set_block_value<std::vector<int>>(std::vector<int>);
template void Block::set_block_value<std::vector<double>>(std::vector<double>);
template void Block::set_block_value<std::vector<std::string>>(std::vector<std::string>);
template void Block::set_block_value<std::vector<std::vector<int>>>(std::vector<std::vector<int>>);
template void Block::set_block_value<std::vector<std::vector<double>>>(std::vector<std::vector<double>>);
template void Block::set_block_value<std::vector<std::vector<std::string>>>(std::vector<std::vector<std::string>>);

} // namespace atomsciflow
