/************************************************************************
MIT License

Copyright (c) 2022 Deqi Tang

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

#ifndef ATOMSCIFLOW_CP2K_POST_PDOS_H_
#define ATOMSCIFLOW_CP2K_POST_PDOS_H_

#include "atomsciflow/post/post.h"
#include "atomsciflow/base/xyz.h"
#include <armadillo>

namespace atomsciflow::cp2k::post {

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

class Pdos : public atomsciflow::post::Post {
public:

    Pdos();
    ~Pdos();

    void set_smear_width(double width) {
        this->smear_width = width;
    }
    void set_smear_npoints(int npoints) {
        this->smear_npoints = npoints;
    }
    void set_xmin(double xmin) {
        this->xmin = xmin;
    }
    void set_xmax(double xmax) {
        this->xmax = xmax;
    }

    virtual void run(const std::string& directory);

private:
    double ha_to_ev;
    double smear_width;
    int smear_npoints;
    double xmin;
    double xmax;
};

} // namespace atomsciflow::cp2k::post

#endif // ATOMSCIFLOW_CP2K_POST_PDOS_H_
