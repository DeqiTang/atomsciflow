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

/// @file src/atomsciflow/vasp/post/phonopy.h
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Mon 28 Feb 2022 11:06:05 PM CST

#ifndef ATOMSCIFLOW_VASP_POST_PHONOPY_H_
#define ATOMSCIFLOW_VASP_POST_PHONOPY_H_

#include <yaml-cpp/yaml.h>

#include "atomsciflow/post/post.h"
#include "atomsciflow/base/crystal.h"
#include "atomsciflow/base/xyz.h"
#include "atomsciflow/base/kpath.h"

namespace atomsciflow::vasp::post {

class Phonopy : public atomsciflow::post::Post {
public:
    Phonopy();
    ~Phonopy(); 

    virtual void run(const std::string& directory);

    void extract_data(const std::string& directory);
    
    void set_kpath(Kpath& kpath) {
        this->kpath = kpath;
    }

    Kpath kpath;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_VASP_POST_PHONOPY_H_
