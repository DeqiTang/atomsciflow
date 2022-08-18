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

from atomsciflow.cpp import octopus
from atomsciflow.cpp.server import JobScheduler
from atomsciflow.cpp import post

class Opt(octopus.PostOpt):
    def __init__(self):
        super().__init__()

class Band(octopus.PostBand):
    def __init__(self):
        super().__init__()

class Dos(post.Post):
    def __init__(self):
        super().__init__()

    def run(self, directory):
        super().run(directory)
        import os
        import numpy as np

        efermi = np.loadtxt(os.path.join(directory, "static/total-dos-efermi.dat"))

        list_of_files = os.listdir(os.path.join(directory, "static"))
        dos_files = []
        for item in list_of_files:
            if item.startswith("dos-"):
                dos_files.append(item)
        if len(dos_files[0].split("-")) == 2:
            num_spins = 1
            num_bands = len(dos_files)
        elif len(dos_files[0].split("-")) == 3:
            num_spins = 2
            num_bands = int(len(dos_files) / 2)
        else:
            num_spins = 1
            num_bands = len(dos_files)

        with open(os.path.join(directory, "post.dir/dos.gnuplot"), "w") as fout:
            fout.write("set terminal png\n")
            fout.write("set parametric\n")
            fout.write("set title 'Density of state' font ',15'\n")
            fout.write("set ylabel 'Density of state' font ',15'\n")
            fout.write("set xlabel 'Energy (eV)' font ',15'\n")
            fout.write("set ytics font ',15'\n")
            fout.write("set xtics font ',15'\n")
            fout.write("set border linewidth 3\n")
            fout.write("set autoscale\n")
            fout.write("set xrange [-10:10]\n")

            fout.write("set arrow from 0, graph 0 to 0, graph 1 nohead linecolor rgb \'black\' linewidth 0.5\n")
            fout.write("set arrow from -10, 0 to 10, 0 nohead linecolor rgb \'black\' linewidth 0.5\n")

            if num_spins == 1:
                fout.write("set output 'total-dos.png'\n")
                fout.write("plot '../static/total-dos.dat' using (column(1)-(%f)):2 w l notitle linewidth 3\n" % efermi[1][0])
                fout.write("set output 'dos-band-resolved.png'\n")
                fout.write("plot \\\n")
                for i in range(num_bands-1):
                    fout.write("  '../static/dos-%04d.dat' using (column(1)-(%f)):($2) w l notitle linewidth 3,\\\n" % (
                        i+1, efermi[1][0]
                    ))
                fout.write("  '../static/dos-%04d.dat' using (column(1)-(%f)):($2) w l notitle linewidth 3\n" % (
                    num_bands, efermi[1][0]
                ))
            elif num_spins == 2:
                fout.write("set output 'total-dos.png'\n")
                fout.write("plot \\\n")
                fout.write("  '../static/total-dos-1.dat' using (column(1)-(%f)):(column(2)) w l notitle linewidth 3,\\\n" % efermi[1][0])
                fout.write("  '../static/total-dos-2.dat' using (column(1)-(%f)):(-column(2)) w l notitle linewidth 3\n" % efermi[1][0])

                fout.write("set output 'dos-band-resolved.png'\n")
                fout.write("plot \\\n")
                for i in range(num_bands-1):
                    fout.write("  '../static/dos-%04d-1.dat' using (column(1)-(%f)):($2) w l notitle linewidth 3,\\\n" % (
                        i+1, efermi[1][0],
                    ))
                    fout.write("  '../static/dos-%04d-2.dat' using (column(1)-(%f)):(-$2) w l notitle linewidth 3,\\\n" % (
                        i+1, efermi[1][0]
                    ))
                fout.write("  '../static/dos-%04d-1.dat' using (column(1)-(%f)):($2) w l notitle linewidth 3,\\\n" % (
                    num_bands, efermi[1][0]
                ))
                fout.write("  '../static/dos-%04d-2.dat' using (column(1)-(%f)):(-$2) w l notitle linewidth 3\n" % (
                    num_bands, efermi[1][0]
                ))

        with open(os.path.join(directory, "post.dir/analysis.sh"), "w") as fout:
            fout.write("#!/bin/bash\n\n")
            fout.write("#\n")
            fout.write("cd %s\n" % os.path.abspath(os.path.join(directory, self.run_params["post-dir"])))
            fout.write("\n")
            fout.write("gnuplot dos.gnuplot\n")

        cmd = ""
        cmd += "bash "
        cmd += os.path.join(directory, self.run_params["post-dir"] + "/analysis.sh")
        os.system(cmd)
