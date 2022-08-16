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

from atomsciflow.cpp import abinit
from atomsciflow.cpp import post

class Phonopy(abinit.PostPhonopy):
    def __init__(self):
        super().__init__()

class Band(post.Post):
    def __init__(self):
        super().__init__()

        self.set_run("program-out", "abinit.abo")
        self.set_run("output-json", "post-band.json")

    def set_kpath(self, kpath):
        self.kpath = kpath

    def set_cell(self, cell):
        self.cell = cell

    def run(self, directory):
        super().run(directory)
        import os
        import numpy as np

        with open(os.path.join(directory, "abinito_DS2_EBANDS.agr"), "r") as fin:
            ebands_lines = fin.readlines()
        
        mband = int(ebands_lines[2].split()[2].replace(",", ""))
        nkpt = int(ebands_lines[2].split()[4].replace(",", ""))
        nsppol = int(ebands_lines[2].split()[6].replace(",", ""))
        nspinor = int(ebands_lines[2].split()[8].replace(",", ""))

        band_data = {}
        bandlines_index = []
        for i in range(len(ebands_lines)):
            if ebands_lines[i].count("@target") == 1:
                bandlines_index.append(i)
        
        num_bands = mband
        num_kpoints = bandlines_index[1] - bandlines_index[0] - 3
        num_spins = int(len(bandlines_index) / num_bands)

        for spin in range(num_spins):
            band_data["spin-%d" % spin] = []
            for band in range(num_bands):
                start = bandlines_index[spin*num_bands + band] + 2
                band_data["spin-%d" % spin].append([float(ebands_lines[i].split()[1]) for i in range(start, start+num_kpoints)])

        kcoords_1d = []

        cell = np.array(self.cell)
        volume = np.dot(cell[0], np.cross(cell[1], cell[2]))
        cell_k = np.zeros([3, 3])
        cell_k[0] = np.cross(cell[1], cell[2]) * 2 * np.pi / volume
        cell_k[1] = np.cross(cell[2], cell[0]) * 2 * np.pi / volume
        cell_k[2] = np.cross(cell[0], cell[1]) * 2 * np.pi / volume

        kcoords_1d.append(0.0000000)
        for i in range(1, len(self.kpath.coords)):
            if self.kpath.links[i-1] != 0:
                point2 = self.kpath.coords[i][0] * cell_k[0] + self.kpath.coords[i][1] * cell_k[1] + self.kpath.coords[i][2] * cell_k[2]
                point1 = self.kpath.coords[i-1][0] * cell_k[0] + self.kpath.coords[i-1][1] * cell_k[1] + self.kpath.coords[i-1][2] * cell_k[2]
                distance_in_k_space = np.linalg.norm(point2 - point1)
                step = distance_in_k_space / (self.kpath.links[i-1])
                for j in range(self.kpath.links[i-1]):
                    kcoords_1d.append(kcoords_1d[-1] + step)
                if i == len(self.kpath.coords) - 1:
                    continue
            else:
                kcoords_1d.append(kcoords_1d[-1])

        for spin in range(num_spins):
            with open(os.path.join(directory, "post.dir/band-spin-%d.data" % spin), "w") as fout:
                for k in range(num_kpoints):
                    fout.write("%f" % kcoords_1d[k])
                    for band in range(num_bands):
                        fout.write(" %f" % band_data["spin-%d" % spin][band][k])
                    fout.write("\n")

        xtics_locs = []
        xtics_labels = []

        k_index = 0
        xtics_locs.append(kcoords_1d[k_index])
        for i in range(1, len(self.kpath.links)):
            if self.kpath.links[i-1] != 0:
                k_index = k_index + self.kpath.links[i-1]
                xtics_locs.append(kcoords_1d[k_index])
            else:
                k_index = k_index + 1

        xtics_labels.append(
            self.kpath.labels[0].upper() if self.kpath.labels[0] != "GAMMA" else "{/symbol G}"
        )
        for i in range(1, len(self.kpath.links)):
            if self.kpath.links[i-1] != 0:
                xtics_labels.append(
                    self.kpath.labels[i].upper() if self.kpath.labels[i] != "GAMMA" else "{/symbol G}"
                )
            else:
                xtics_labels[-1] = xtics_labels[-1] + " | " + self.kpath.labels[i].upper() if self.kpath.labels[i] != "GAMMA" else "{/symbol G}"

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

            for spin in range(num_spins):
                fout.write("set output 'band-spin-%d.png'\n" % spin)
                fout.write("plot for [j=2:%d] \'band-spin-%d.data\' using 1:(column(j)) w l notitle linestyle 1\n" % (num_bands+1, spin))

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
