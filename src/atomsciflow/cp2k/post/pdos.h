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

class ElementPdos {
public:

    ElementPdos(const std::string& pdos_file);
    ~ElementPdos() {
    };

    void get_data();

    std::string get_spin();

    std::string pdos_file;
    std::string spin;
    std::string kind;
    double fermi; // in a.u.
    // used to convert energy from [a.u.] unit to [eV] unit
    double ha_to_ev;
    arma::mat data;
    std::vector<std::string> orbitals;
};

class Pdos : public atomsciflow::post::Post {
public:

    Pdos();
    ~Pdos();

    virtual void run(const std::string& directory);

    std::vector<std::shared_ptr<ElementPdos>> data;
    std::vector<std::shared_ptr<ElementPdos>> data_smearing;

};

} // namespace atomsciflow::cp2k::post

#endif // ATOMSCIFLOW_CP2K_POST_PDOS_H_
