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

/// @file src/atomsciflow/base/kpath.h
/// @author Deqi Tang
/// Mail: deqi_tang@163.com
/// Created Time: Mon 25 Jan 2021 07:39:31 PM CST

#ifndef ATOMSCIFLOW_BASE_KPATH_H_
#define ATOMSCIFLOW_BASE_KPATH_H_

#include <vector>
#include <string>

namespace atomsciflow {

/**
 * The high symmetry k point path used in bands structure calculation
 * in format like this:
 *     {{kx, ky, kz, label, link}, ...} like {{0.0, 0.0, 0.0, 'GAMMA', 15}, ...}
 *     described by a list in python, and a vector in c++
 * if link in a kpoint is an integer larger than 0, then it will connect to the following point
 * through the number of kpoints defined by link.
 * if link in a kpoint is an integer 0, then it will not connect to the following point,
 */
class Kpath {
public:
    Kpath() {};
    ~Kpath() {};

    void add_point(float kx, float ky, float kz, std::string label, int link) {
        this->coords.push_back(std::vector<double>{kx, ky, kz});
        this->labels.push_back(label);
        this->links.push_back(link);
        return ;
    }

    std::vector<std::vector<double> > coords;
    std::vector<std::string> labels;
    std::vector<int> links;
    int nkpoint;
};

Kpath get_kpath(const std::vector<std::string>& kpath_manual, const std::string& kpath_file); 

} // namespace atomsciflow

#endif // ATOMSCIFLOW_BASE_KPATH_H_
