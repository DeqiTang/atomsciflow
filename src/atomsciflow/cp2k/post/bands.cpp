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

#include "atomsciflow/cp2k/post/bands.h"

#include <boost/algorithm/string.hpp>

namespace atomsciflow::cp2k::post {

namespace ba = boost::algorithm;

Bands::Bands() {    
    this->ha_to_ev = 27.211324570273; 
}

Bands::~Bands() {

}

void Bands::run(const std::string& directory) {

    // get fermi energy
    std::ifstream in_f;
    in_f.open((fs::path(directory) / "cp2k.out").string());
    in_f.close();

    // get band data
    in_f.open((fs::path(directory) / "bands.bs").string());

    in_f.close();

    // export smearing pdos data
    fs::create_directory(fs::path(directory) / run_params["post-dir"]);

    std::ofstream stream;    

    stream.open((fs::path(directory) / run_params["post-dir"] / "plot.gnuplot").string());
    stream << "set terminal png\n"
        << "set parametric\n"
        << "set title 'Band Structure'\n"
        << "set ylabel 'Energy (eV)'\n"
        << "set grid xtics ytics\n"
        << "set autoscale\n"
        // << "set yrange [-10:10]\n";
        ;
    stream.close();

    stream.open((fs::path(directory) / run_params["post-dir"] / "analysis.sh").string());
    stream << "#!/bin/bash\n\n"
        << "#\n"
        << boost::format("cd %1%\n") % (fs::absolute(fs::path(directory)) / run_params["post-dir"]).string()
        << "\n"
        << "gnuplot plot.gnuplot\n";
    stream.close();

    std::string cmd = "";
    cmd += "bash ";
    cmd += (fs::path(directory) / run_params["post-dir"] / "analysis.sh").string();
    std::system(cmd.c_str());
}

} // namespace atomsciflow::cp2k::post
