"""
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
"""

import os

from atomsciflow.cpp import elk
from atomsciflow.cpp import post

class Opt(elk.PostOpt):
    def __init__(self):
        super().__init__()

class Band(post.Post):
    def __init__(self):
        super().__init__()

    def set_kpath(self, kpath):
        self.kpath = kpath

    def run(self, directory):
        super().run(directory)

        with open(os.path.join(directory, "BANDLINES.OUT") ,"r") as fin:
            bandlines_out = fin.readlines()

        xtics_locs = []
        xtics_locs.append(float(bandlines_out[0].split()[0]))
        for i in range(1, len(bandlines_out)):
            if len(bandlines_out[i].split()) == 0:
                continue
            if (float(bandlines_out[i].split()[0]) - xtics_locs[-1]) < 1.0e-5:
                continue
            else:
                xtics_locs.append(float(bandlines_out[i].split()[0]))

        xtics_labels = []
        for label in self.kpath.labels:
            if label != "GAMMA":
                xtics_labels.append(label)
            else:
                xtics_labels.append("{/symbol G}")

        with open(os.path.join(directory, "post.dir/band.gnuplot"), "w") as fout:
            fout.write("set terminal png\n")
            fout.write("unset key\n")
            fout.write("set parametric\n")
            fout.write("set title 'Band structure' font ',15'\n")
            fout.write("set ylabel 'Energy (eV)' font ',15'\n")
            fout.write("set ytics font ',15'\n")
            fout.write("set xtics font ',15'\n")
            fout.write("set border linewidth 3\n")
            fout.write("set autoscale\n")
            # set a linestyle 1 to be the style for band lines
            fout.write("set style line 1 linecolor rgb \'black\' linetype 1 pointtype 1 linewidth 3\n")
            # set a linestyle 2 to be the style for the vertical high-symmetry kpoint line and horizontal fermi level line
            fout.write("set style line 2 linecolor rgb \'black\' linetype 1 pointtype 2 linewidth 0.5\n")
            
            fout.write("set xtics(")
            for i in range(len(xtics_locs) - 1):
                fout.write("\'%s\' %f, "% (xtics_labels[i], xtics_locs[i]))
            fout.write("\'%s\' %f)\n" % (xtics_labels[-1], xtics_locs[-1]))
            
            for x in xtics_locs:
                fout.write("set arrow from %f, graph 0 to %f, graph 1 nohead linestyle 2\n" % (x, x))
            fout.write("set arrow from 0, 0 to %f, 0 nohead linestyle 2\n" % xtics_locs[-1])

            fout.write("set output 'band-structure.png'\n")
            fout.write("plot \'../BAND.OUT\' using 1:(column(2)) w l notitle linestyle 1\n")

        with open(os.path.join(directory, "post.dir/analysis.sh"), "w") as fout:
            fout.write("#!/bin/bash\n\n")
            fout.write("#\n")
            fout.write("cd %s\n" % os.path.abspath(os.path.join(directory, self.run_params["post-dir"])))
            fout.write("\n")
            fout.write("gnuplot band.gnuplot\n")

        cmd = ""
        cmd += "bash "
        cmd += os.path.join(directory, self.run_params["post-dir"] + "/analysis.sh")
        os.system(cmd)
