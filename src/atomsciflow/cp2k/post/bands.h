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

#ifndef ATOMSCIFLOW_CP2K_POST_BANDS_H_
#define ATOMSCIFLOW_CP2K_POST_BANDS_H_

#include <memory>
#include <armadillo>

#include "atomsciflow/post/post.h"
#include "atomsciflow/base/kpath.h"

namespace atomsciflow::cp2k::post {

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

struct Kpoint {
    arma::rowvec coord;
    std::string spin;
    arma::rowvec band;
    arma::rowvec energy;
    arma::rowvec occupation;
};

struct KpointSet {
    int num_kpoints;
    int num_bands;
    std::string special_label_1;
    std::string special_label_2;
    arma::rowvec special_coord_1;
    arma::rowvec special_coord_2;
    std::vector<std::shared_ptr<Kpoint>> kpoint;
};

class Bands : public atomsciflow::post::Post {
public:

    Bands();
    ~Bands();

    void set_ymin(double ymin) {
        this->ymin = ymin;
    }

    void set_ymax(double ymax) {
        this->ymax = ymax;
    }

    virtual void run(const std::string& directory);
    
    // used to convert energy from [a.u.] unit to [eV] unit
    double ha_to_ev;
    std::vector<std::shared_ptr<KpointSet>> kpoint_sets;
    double ymin;
    double ymax;
};

} // namespace atomsciflow::cp2k::post

#endif // ATOMSCIFLOW_CP2K_POST_BANDS_H_
