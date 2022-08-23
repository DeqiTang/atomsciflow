"""
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
"""

from atomsciflow.cpp import qe
from atomsciflow.cpp.base import Xyz, Kpath
from atomsciflow.cpp.server import JobScheduler
from atomsciflow.cpp.config import ConfigManager

class PwScf(qe.PwScf):
    def __init__(self):
        super().__init__()

class Static(PwScf):
    def __init__(self):
        super().__init__()

    def run(self, directory):
        super().run(directory)
        
class Opt(PwScf):
    def __init__(self):
        super().__init__()
        
        self.set_param("control", "calculation", "relax")
        self.set_param("control", "title", "structure relaxation")
        self.set_param("control", "nstep", 200)
        self.set_param("control", "etot_conv_thr", 1.0e-6)
        self.set_param("control", "forc_conv_thr", 1.0e-5)
        
    def run(self, directory):
        super().run(directory)

class VcOpt(PwScf):
    def __init__(self):
        super().__init__()
        
        self.set_param("control", "calculation", "vc-relax")
        self.set_param("control", "title", "cell optimization")
        self.set_param("control", "nstep", 200)
        self.set_param("control", "etot_conv_thr", 1.0e-6)
        self.set_param("control", "forc_conv_thr", 1.0e-5)   

        self.set_param("cell", "cell_dynamics", "bfgs")
        self.set_param("cell", "press_conv_thr", 0.5)
        self.set_param("cell", "cell_dofree", "all")

class Phonopy(qe.Phonopy):
    def __init__(self):
        super().__init__()

class Band(PwScf):
    def __init__(self):
        super().__init__()

    def set_kpath(self, kpath):
        self.kpath = kpath

    def run(self, directory):
        import os
        import numpy as np
        self.set_param("control", "wf_collect", ".true.")

        step = ""
        step += "cd ${ABSOLUTE_WORK_DIR}\n"
        for item in self.xyz.elements_set:
            step += "# pseudopotential file for element: " + item + "\n"
            step += "for item in %s/*\n" % os.path.join(self.config.get_pseudo_pot_dir()["qe"], "SSSP_efficiency_pseudos")
            step += "do\n"
            if len(item) == 1:
                step += "if [[ ${item} =~ /[%s|%s][.|_] ]]\n" % (item.upper(), item.lower())
            else:
                step += "if [[ ${item} =~ /[%s|%s][%s|%s][.|_] ]]\n" % (
                    item[0].upper(), 
                    item[0].lower(), 
                    item[1].upper(), 
                    item[1].lower()
                )
            step += "then\n"
            step += "cp ${item} ${ABSOLUTE_WORK_DIR}/\n"
            step += "fi\n"
            step += "done\n"
        self.job.append_step(step)

        step = ""
        self.set_param("control", "calculation", "scf")
        step += "cat >pw-scf.in<<EOF\n"
        step += self.to_string()
        step += "EOF\n"
        step += "$CMD_HEAD %s < pw-scf.in | tee pw-scf.out\n" % (
            self.job.run_params["cmd"],
        )
        self.job.append_step(step)

        step = ""
        self.set_param("control", "calculation", "nscf")
        if self.get_card_option("k_points") == "automatic":
            k_scf = self.get_card_data("k_points")[0]
            k_nscf = []
            for k in k_scf:
                k_nscf.append(int(k))
            for i in range(3):
                k_nscf[i] = int(np.ceil(k_nscf[i] * 1.333))
            self.set_card_data("k_points(automatic)", k_nscf, 0)
        step += "cat >pw-nscf.in<<EOF\n"
        step += self.to_string()
        step += "EOF\n"
        step += "$CMD_HEAD %s < pw-nscf.in | tee pw-nscf.out\n" % (
            self.job.run_params["cmd"],
        )
        self.job.append_step(step)

        step = ""
        self.set_param("control", "calculation", "bands")
        self.set_card_option("k_points", "crystal_b")
        self.set_card_data("k_points", [len(self.kpath.labels)], 0)
        for i in range(len(self.kpath.coords)):
            self.set_card_data("k_points", "%7.5f"%self.kpath.coords[i][0], i+1, 0)
            self.set_card_data("k_points", "%7.5f"%self.kpath.coords[i][1], i+1, 1)
            self.set_card_data("k_points", "%7.5f"%self.kpath.coords[i][2], i+1, 2)
            self.set_card_data("k_points", self.kpath.links[i], i+1, 3)
            self.set_card_data("k_points", "#%s"%self.kpath.labels[i], i+1, 4)
        step += "cat >pw-bands.in<<EOF\n"
        step += self.to_string()
        step += "EOF\n"
        step += "$CMD_HEAD %s < pw-bands.in | tee pw-bands.out\n" % (
            self.job.run_params["cmd"],
        )
        self.job.append_step(step)

        step = ""
        step += "cat >bands.in<<EOF\n"
        step += "&BANDS\n"
        # step += "prefix = \'pwscf\'\n"
        # step += "outdir = \'./tmp\'\n"
        step += "filband = \'banddata.out\'\n"
        step += "lsym = .true.\n"
        step += "/\n"
        step += "EOF\n"
        step += "$CMD_HEAD $ASF_CMD_QE_BANDSX < bands.in | tee bands.out\n"
        self.job.append_step(step)

        self.job.run(directory)

class Dos(PwScf):
    def __init__(self):
        super().__init__()

    def run(self, directory):
        import os
        import numpy as np
        self.set_param("control", "wf_collect", ".true.")

        step = ""
        step += "cd ${ABSOLUTE_WORK_DIR}\n"
        for item in self.xyz.elements_set:
            step += "# pseudopotential file for element: " + item + "\n"
            step += "for item in %s/*\n" % os.path.join(self.config.get_pseudo_pot_dir()["qe"], "SSSP_efficiency_pseudos")
            step += "do\n"
            if len(item) == 1:
                step += "if [[ ${item} =~ /[%s|%s][.|_] ]]\n" % (item.upper(), item.lower())
            else:
                step += "if [[ ${item} =~ /[%s|%s][%s|%s][.|_] ]]\n" % (
                    item[0].upper(), 
                    item[0].lower(), 
                    item[1].upper(), 
                    item[1].lower()
                )
            step += "then\n"
            step += "cp ${item} ${ABSOLUTE_WORK_DIR}/\n"
            step += "fi\n"
            step += "done\n"
        self.job.append_step(step)

        # scf
        step = ""
        self.set_param("control", "calculation", "scf")
        step += "cat >pw-scf.in<<EOF\n"
        step += self.to_string()
        step += "EOF\n"
        step += "$CMD_HEAD %s < pw-scf.in | tee pw-scf.out\n" % (
            self.job.run_params["cmd"],
        )
        self.job.append_step(step)

        # nscf
        step = ""
        self.set_param("control", "calculation", "nscf")
        if self.get_card_option("k_points") == "automatic":
            k_scf = self.get_card_data("k_points")[0]
            k_nscf = []
            for k in k_scf:
                k_nscf.append(int(k))
            for i in range(3):
                k_nscf[i] = int(np.ceil(k_nscf[i] * 1.333))
            self.set_card_data("k_points(automatic)", k_nscf, 0)
        step += "cat >pw-nscf.in<<EOF\n"
        step += self.to_string()
        step += "EOF\n"
        step += "$CMD_HEAD %s < pw-nscf.in | tee pw-nscf.out\n" % (
            self.job.run_params["cmd"],
        )
        self.job.append_step(step)

        # projwfc
        step = ""
        step += "cat >projwfc.in<<EOF\n"
        step += "&PROJWFC\n"
        # step += "prefix = \'pwscf\'\n"
        # step += "outdir = \'./tmp\'\n"
        step += "ngauss = 0\n"
        step += "degauss = 0.01\n"
        step += "/\n"
        step += "EOF\n"
        step += "$CMD_HEAD $ASF_CMD_QE_PROJWFCX < projwfc.in | tee projwfc.out\n"
        self.job.append_step(step)

        self.job.run(directory)

class BOMD(PwScf):
    def __init__(self):
        super().__init__()
        
        self.set_param("control", "calculation", "md")
        self.set_param("control", "title", "Born-Oppenheimer MD")
        self.set_param("control", "nstep", 1000)
        self.set_param("ions", "ion_dynamics", "verlet")
        self.set_param("ions", "ion_temperature", "andersen")
        self.set_param("ions", "tempw", 300.0)
        self.set_param("ions", "wfc_extrapolation", "second_order")
        