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

/// @file src/atomsciflow/vasp/post/bands.h
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Wed 09 Mar 2022 09:58:59 AM CST

#ifndef ATOMSCIFLOW_VASP_POST_BANDS_H_
#define ATOMSCIFLOW_VASP_POST_BANDS_H_

#include <vector>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>

#include "atomsciflow/base/kpath.h"

namespace atomsciflow {

namespace pt = boost::property_tree;
namespace ba = boost::algorithm;

/**
 * @class PostBands
 *
 */
class PostBands {
public:
    void get_efermi(std::string vasprun);
    void get_kpath_and_vasprun(Kpath kpath, std::string vasprun);
    void get_xcoord_k();
    void get_eigenval();
    void _plot_band_gnuplot(std::vector<double>& bandrange, std::vector<double>& xrange, std::vector<double>& yrange);
    void process(std::string directory, std::vector<double>& bandrange, std::vector<double>& xrange, std::vector<double>& yrange);

    pt::ptree vasprun_root;
    Kpath kpath;
    std::vector<double> locs;
    std::vector<std::string> labels_for_gnuplot;
    std::vector<double> xcoord_k;
    double efermi;
};

} // namespace atomsciflow

#endif // ATOMSCIFLOW_VASP_POST_BANDS_H_
