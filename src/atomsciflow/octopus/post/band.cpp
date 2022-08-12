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

#include "atomsciflow/octopus/post/band.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <armadillo>

namespace atomsciflow::octopus::post {

namespace fs = boost::filesystem;
namespace ba = boost::algorithm;

Band::Band() {
    this->set_run("program-out", "octopus-gs.out");
    // this->set_run("program-out", "octopus-unocc.out");
    this->set_run("output-json", "post-band.json");
}

void Band::run(const std::string& directory) {
    Post::run(directory);

    arma::mat band;
    std::ifstream instream;
    instream.open((fs::path(directory) / "static/bandstructure").string());
    std::string line;
    // read the first comment line of bandstructure file stream 
    // so that we can use armadillo to load the data correctly
    std:;getline(instream, line);
    band.load(instream);
    instream.close();

    std::vector<double> xtics_locs;
    std::vector<std::string> xtics_labels;
    int high_sym_index = 0;
    xtics_locs.emplace_back(band.at(high_sym_index, 0));
    for (int i = 0; i < this->kpath.links.size() - 1; i++) {
        if (kpath.links[i] != 0) {
            high_sym_index += kpath.links[i];
            xtics_locs.emplace_back(band.at(high_sym_index, 0));
        }
    }

    xtics_labels.emplace_back(
        ba::to_upper_copy(this->kpath.labels[0]) != "GAMMA" ? ba::to_upper_copy(this->kpath.labels[0]) : "{/symbol G}"
    );
    for (int i = 1; i < this->kpath.coords.size(); i++) {
        if (this->kpath.links[i-1] != 0) {
            xtics_labels.emplace_back(
                ba::to_upper_copy(this->kpath.labels[i]) != "GAMMA" ? this->kpath.labels[i] : "{/symbol G}"
            );
        } else {
            xtics_labels[xtics_labels.size()-1] = xtics_labels[xtics_labels.size()-1] + " | " + ba::to_upper_copy(this->kpath.labels[i]);
        }
    }

    std::ofstream stream;

    stream.open((fs::path(directory) / run_params["post-dir"] / "band.gnuplot").string());
    stream << "set terminal png\n";
    stream << "set parametric\n";
    stream << "set title 'Band structure' font ',15'\n";
    stream << "set ylabel 'Energy (eV)' font ',15'\n";
    stream << "set ytics font ',15'\n";
    stream << "set xtics font ',15'\n";
    stream << "set border linewidth 3\n";
    stream << "set autoscale\n";
    stream << "set output 'band-structure.png'\n";
    stream << "set yrange [-10:30]\n";
    // set a linestyle 1 to be the style for band lines
    stream << "set style line 1 linecolor rgb \'black\' linetype 1 pointtype 1 linewidth 3\n";
    // set a linestyle 2 to be the style for the vertical high-symmetry kpoint line and horizontal fermi level line
    stream << "set style line 2 linecolor rgb \'black\' linetype 1 pointtype 2 linewidth 0.5\n";
        
    stream << "set xtics(";
    for (int i = 0; i < xtics_labels.size()-1; i++) {
        stream << boost::format("\'%1%\' %2%, ") % xtics_labels[i] % xtics_locs[i];
    }
    stream << boost::format("\'%1%\' %2%)\n") % xtics_labels[xtics_labels.size()-1] % xtics_locs[xtics_locs.size()-1];
    
    for (auto& x : xtics_locs) {
        stream << boost::format("set arrow from %1%, graph 0 to %1%, graph 1 nohead linestyle 2\n") % x;
    }
    stream << boost::format("set arrow from 0, 0 to %1%, 0 nohead linestyle 2\n") % xtics_locs[xtics_locs.size()-1];
            
    stream << "plot for [j=5:*] \'../static/bandstructure\' u 1:j w l notitle linestyle 1\n";
    stream.close();

    stream.open((fs::path(directory) / run_params["post-dir"] / "analysis.sh").string());
    stream << "#!/bin/bash\n\n"
        << "#\n"
        << boost::format("cd %1%\n") % (fs::absolute(fs::path(directory)) / run_params["post-dir"]).string()
        << "\n"
        << "gnuplot band.gnuplot\n";
    stream.close();

    std::string cmd = "";
    cmd += "bash ";
    cmd += (fs::path(directory) / run_params["post-dir"] / "analysis.sh").string();
    std::system(cmd.c_str());

}

} // namespace atomsciflow::octopus::post
