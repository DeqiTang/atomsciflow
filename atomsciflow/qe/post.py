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

from atomsciflow.cpp import qe
from atomsciflow.cpp import post
class Phonopy(qe.PostPhonopy):
    def __init__(self):
        super().__init__()

class Opt(qe.PostOpt):
    def __init__(self):
        super().__init__()

class Band(post.Post):
    def __init__(self):
        super().__init__()

    def set_kpath(self, kpath):
        self.kpath = kpath

    def run(self, directory):
        super().run(directory)

        import os

        with open(os.path.join(directory, "pw-nscf.out"), 'r') as fin:
            for line in fin:
                if len(line.split()) == 0:
                    continue
                if line.count("the Fermi") > 0:
                    self.fermi_energy = float(line.split()[4])

        with open(os.path.join(directory, "bands.out"), 'r') as fout:
            self.bands_out = fout.readlines()

        high_sym_index_begin = 0
        for i in range(len(self.bands_out)):
            if self.bands_out[i].count("high-symmetry") > 0:
                high_sym_index_begin = i
                break

        for i in range(len(self.bands_out)):
            if self.bands_out[i].count("Plottable") > 0:
                self.bandfile_gnu = self.bands_out[i].split()[6]

        high_sym_xcoord = []
        for i in range(len(self.kpath.links)):
            high_sym_xcoord.append(float(self.bands_out[high_sym_index_begin+i].split()[-1].split("\n")[0]))

        xtics_locs = []
        xtics_locs.append(high_sym_xcoord[0])
        for i in range(1, len(high_sym_xcoord)):
            if abs(high_sym_xcoord[i] - high_sym_xcoord[i-1]) <1.0e-5:
                continue
            else:
                xtics_locs.append(high_sym_xcoord[i])
        
        xtics_labels = []
        xtics_labels.append(self.kpath.labels[0] if self.kpath.labels[0] != "GAMMA" else "{/symbol G}")
        for i in range(1, len(self.kpath.labels)):
            if (self.kpath.links[i-1] == 0):
                xtics_labels[-1] = xtics_labels[-1] + "|" + (self.kpath.labels[i] if self.kpath.labels[i] != "GAMMA" else "{/symbol G}")
            else:
                xtics_labels.append(self.kpath.labels[i] if self.kpath.labels[i] != "GAMMA" else "{/symbol G}")

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
            fout.write("plot \'../%s\' using 1:(column(2) - %f) w l notitle linestyle 1\n" % (self.bandfile_gnu, self.fermi_energy))

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


class Dos(post.Post):
    def __init__(self):
        super().__init__()

    def run(self, directory):
        super().run(directory)

        import os
        import numpy as np

        with open(os.path.join(directory, "pw-nscf.out"), 'r') as fin:
            for line in fin:
                if len(line.split()) == 0:
                    continue
                if line.count("the Fermi") > 0:
                    self.fermi_energy = float(line.split()[4])
                    break

        pdos_tot = np.loadtxt(os.path.join(directory, "pwscf.pdos_tot"))
        if pdos_tot.shape[1] == 3:
            num_spins = 1
        elif pdos_tot.shape[1] == 5:
            num_spins = 2
        else:
            num_spins = 1
        energies = pdos_tot[:, 0]

        list_of_files = os.listdir(directory)

        pdos_files = []
        for item in list_of_files:
            if item.count("pwscf.pdos_atm#") > 0:
                pdos_files.append(item)

        map_l_m = {}
        map_l_m["s"] = [0]
        map_l_m["p"] = [-1, 0, 1]
        map_l_m["d"] = [-2, -1, 0, 1, 2]
        map_l_m["f"] = [-3, -2, -1, 0, 1, 2, 3]

        elem_proj = {}
        for item in pdos_files:
            elem = item.split("_")[1].split("(")[1].split(")")[0]
            l = item.split("(")[2].replace(")", "") # angular momentum
            data = np.loadtxt(os.path.join(directory, item))
            if num_spins == 2:
                if not "-".join([elem, "up"]) in elem_proj:
                    elem_proj["-".join([elem, "up"])] = np.zeros(len(energies))
                    elem_proj["-".join([elem, "dw"])] = np.zeros(len(energies))
                for m_i in range(len(map_l_m[l])):
                    elem_proj["-".join([elem, "up"])] = elem_proj["-".join([elem, "up"])] + data[:, 3+m_i*2]
                    elem_proj["-".join([elem, "dw"])] = elem_proj["-".join([elem, "dw"])] + data[:, 3+m_i*2+1]
            else:
                if not elem in elem_proj:
                    elem_proj[elem] = np.zeros(len(energies))
                for m_i in range(len(map_l_m[l])):
                    elem_proj[elem] = elem_proj[elem] + data[:, 2+m_i]

        for item in elem_proj:
            with open(os.path.join(directory, "post.dir/elem-proj-%s.data" % item), "w") as fout:
                fout.write("# Energy(eV) DOS\n")
                for i in range(len(energies)):
                    fout.write("%f %f\n" % (energies[i], elem_proj[item][i]))

        elem_l_proj = {}
        for item in pdos_files:
            elem = item.split("_")[1].split("(")[1].split(")")[0]
            n = item.split("#")[2].split("(")[0]
            l = item.split("(")[2].replace(")", "") # angular momentum
            data = np.loadtxt(os.path.join(directory, item))
            if num_spins == 2:
                if not "-".join([elem, n+l, "up"]) in elem_l_proj:
                    elem_l_proj["-".join([elem, n+l, "up"])] = np.zeros(len(energies))
                    elem_l_proj["-".join([elem, n+l, "dw"])] = np.zeros(len(energies))
                for m_i in range(len(map_l_m[l])):
                    elem_l_proj["-".join([elem, n+l, "up"])] = elem_l_proj["-".join([elem, n+l, "up"])] + data[:, 3+m_i*2]
                    elem_l_proj["-".join([elem, n+l, "dw"])] = elem_l_proj["-".join([elem, n+l, "dw"])] + data[:, 3+m_i*2+1]
            else:
                if not "-".join([elem, n+l]) in elem_l_proj:
                    elem_l_proj["-".join([elem, n+l])] = np.zeros(len(energies))
                for m_i in range(len(map_l_m[l])):
                    elem_l_proj["-".join([elem, n+l])] = elem_l_proj["-".join([elem, n+l])] + data[:, 2+m_i]

        for item in elem_l_proj:
            with open(os.path.join(directory, "post.dir/elem-l-proj-%s.data" % item), "w") as fout:
                fout.write("# Energy(eV) DOS\n")
                for i in range(len(energies)):
                    fout.write("%f %f\n" % (energies[i], elem_l_proj[item][i]))

        elem_l_m_proj = {}
        for item in pdos_files:
            elem = item.split("_")[1].split("(")[1].split(")")[0]
            n = item.split("#")[2].split("(")[0]
            l = item.split("(")[2].replace(")", "") # angular momentum
            data = np.loadtxt(os.path.join(directory, item))
            if num_spins == 2:
                for m_i in range(len(map_l_m[l])):
                    m = map_l_m[l][m_i]
                    if not "-".join([elem, n+l+"(%d)" % m, "up"]) in elem_l_m_proj:
                        elem_l_m_proj["-".join([elem, n+l+"(%d)" % m, "up"])] = np.zeros(len(energies))
                        elem_l_m_proj["-".join([elem, n+l+"(%d)" % m, "dw"])] = np.zeros(len(energies))
                    elem_l_m_proj["-".join([elem, n+l+"(%d)" % m, "up"])] = elem_l_m_proj["-".join([elem, n+l+"(%d)" % m, "up"])] + data[:, 3+m_i*2]
                    elem_l_m_proj["-".join([elem, n+l+"(%d)" % m, "dw"])] = elem_l_m_proj["-".join([elem, n+l+"(%d)" % m, "dw"])] + data[:, 3+m_i*2+1]
            else:
                for m_i in range(len(map_l_m[l])):
                    m = map_l_m[l][m_i]
                    if not "-".join([elem, n+l+"(%d)" % m]) in elem_l_m_proj:
                        elem_l_m_proj["-".join([elem, n+l+"(%d)" % m])] = np.zeros(len(energies))
                    elem_l_m_proj["-".join([elem, n+l+"(%d)" % m])] = elem_l_m_proj["-".join([elem, n+l+"(%d)" % m])] + data[:, 2+m_i]

        for item in elem_l_m_proj:
            with open(os.path.join(directory, "post.dir/elem-l-m-proj-%s.data" % item), "w") as fout:
                fout.write("# Energy(eV) DOS\n")
                for i in range(len(energies)):
                    fout.write("%f %f\n" % (energies[i], elem_l_m_proj[item][i]))

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
                fout.write("plot '../pwscf.pdos_tot' using (column(1)-(%f)):2 w l notitle linecolor rgb \'black\' linewidth 3\n" % (self.fermi_energy))

                fout.write("set output 'elem-proj-dos.png'\n")
                fout.write("plot \\\n")
                i = 0
                for item in elem_proj:
                    if i != len(elem_proj) - 1:
                        fout.write("  'elem-proj-%s.data' using (column(1)-(%f)):(column(2)) w l title '%s' linewidth 3,\\\n" % (
                            item, 
                            self.fermi_energy,
                            item
                        ))
                    else:
                        fout.write("  'elem-proj-%s.data' using (column(1)-(%f)):(column(2)) w l title '%s' linewidth 3\n" % (
                            item, 
                            self.fermi_energy,
                            item
                        ))
                    i = i + 1

                fout.write("set output 'elem-l-proj-dos.png'\n")
                fout.write("plot \\\n")
                i = 0
                for item in elem_l_proj:
                    if i != len(elem_l_proj) - 1:
                        fout.write("  'elem-l-proj-%s.data' using (column(1)-(%f)):(column(2)) w l title '%s' linewidth 3,\\\n" % (
                            item,
                            self.fermi_energy,
                            item
                        ))
                    else:
                        fout.write("  'elem-l-proj-%s.data' using (column(1)-(%f)):(column(2)) w l title '%s' linewidth 3\n" % (
                            item,
                            self.fermi_energy,
                            item
                        ))
                    i = i + 1

                fout.write("set output 'elem-l-m-proj-dos.png'\n")
                fout.write("plot \\\n")
                i = 0
                for item in elem_l_m_proj:
                    if i != len(elem_l_m_proj) - 1:
                        fout.write("  'elem-l-m-proj-%s.data' using (column(1)-(%f)):(column(2)) w l title '%s' linewidth 3,\\\n" % (
                            item,
                            self.fermi_energy,
                            item
                        ))
                    else:
                        fout.write("  'elem-l-m-proj-%s.data' using (column(1)-(%f)):(column(2)) w l title '%s' linewidth 3\n" % (
                            item,
                            self.fermi_energy,
                            item
                        ))
                    i = i + 1
                
            elif num_spins == 2:                    
                fout.write("set output 'total-dos.png'\n")
                fout.write("plot '../pwscf.pdos_tot' using (column(1)-(%f)):(column(2)) w l notitle linecolor rgb \'black\' linewidth 3,\\\n" % (self.fermi_energy))
                fout.write("  '../pwscf.pdos_tot' using (column(1)-(%f)):(-column(3)) w l notitle linecolor rgb \'black\' linewidth 3\n" % (self.fermi_energy))

                fout.write("set output 'elem-proj-dos.png'\n")
                i = 0
                fout.write("plot \\\n")
                for item in elem_proj:
                    if i != len(elem_proj) -1:
                        fout.write("  'elem-proj-%s.data' using (column(1)-(%f)):(column(2)*(%f)) w l title '%s' linewidth 3,\\\n" % (
                            item, 
                            self.fermi_energy,
                            1 if item.split("-")[-1] == "up" else -1,
                            item,
                        ))
                    else:
                        fout.write("  'elem-proj-%s.data' using (column(1)-(%f)):(column(2)*(%f)) w l title '%s' linewidth 3\n" % (
                            item, 
                            self.fermi_energy,
                            1 if item.split("-")[-1] == "up" else -1,
                            item
                        ))
                    i = i + 1

                fout.write("set output 'elem-l-proj-dos.png'\n")
                fout.write("plot \\\n")
                i = 0
                for item in elem_l_proj:
                    if i != len(elem_l_proj) - 1:
                        fout.write("  'elem-l-proj-%s.data' using (column(1)-(%f)):(column(2)*(%f)) w l title '%s' linewidth 3,\\\n" % (
                            item, 
                            self.fermi_energy,
                            1 if item.split("-")[-1] == "up" else -1,
                            item,
                        ))
                    else:
                        fout.write("  'elem-l-proj-%s.data' using (column(1)-(%f)):(column(2)*(%f)) w l title '%s' linewidth 3\n" % (
                            item, 
                            self.fermi_energy,
                            1 if item.split("-")[-1] == "up" else -1,
                            item,
                        ))
                    i = i + 1

                fout.write("set output 'elem-l-m-proj-dos.png'\n")
                fout.write("plot \\\n")
                i = 0
                for item in elem_l_m_proj:
                    if i != len(elem_l_m_proj) - 1:
                        fout.write("  'elem-l-m-proj-%s.data' using (column(1)-(%f)):(column(2)*(%f)) w l title '%s' linewidth 3,\\\n" % (
                            item, 
                            self.fermi_energy,
                            1 if item.split("-")[-1] == "up" else -1,
                            item,
                        ))
                    else:
                        fout.write("  'elem-l-m-proj-%s.data' using (column(1)-(%f)):(column(2)*(%f)) w l title '%s' linewidth 3\n" % (
                            item, 
                            self.fermi_energy,
                            1 if item.split("-")[-1] == "up" else -1,
                            item,
                        ))
                    i = i + 1

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
