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

from atomsciflow.cpp import vasp
from atomsciflow.cpp.server import JobScheduler
from atomsciflow.cpp.config import ConfigManager
from atomsciflow.cpp.base import Kpath

class Vasp(vasp.Vasp):
    def __init__(self):
        super().__init__()

class Static(Vasp):
    def __init__(self):
        super().__init__()
        self.incar.set_runtype("static")
        self.set_param("IBRION", -1)

class Opt(Vasp):
    def __init__(self):
        super().__init__()
        self.incar.set_runtype("opt")
        self.set_param("EDIFFG", 1.0E-3)
        self.set_param("IBRION", 1)
        self.set_param("NSW", 100)

class VcOpt(Vasp):
    def __init__(self):
        super().__init__()
        self.set_param("EDIFFG", 1.0E-3)
        self.set_param("IBRION", 1)
        self.set_param("NSW", 100)
        self.set_param("ISIF", 3)

class MD(Vasp):
    def __init__(self):
        super().__init__()
        self.incar.set_runtype("md")
        self.set_param("IBRION", 0)
        self.set_param("POTIM", 0.5)
        self.set_param("NSW", 1000)

class Phonopy(vasp.Phonopy):
    def __init__(self):
        super().__init__()

class Band(Vasp):
    def __init__(self):
        super().__init__()

    def set_kpath(self, kpath):
        self.kpath = kpath

    def run(self, directory):
        import os

        self.set_param("IBRION", -1)

        step = ""
        step += "cd ${ABSOLUTE_WORK_DIR}\n"
        step += "cat"
        for item in self.poscar.elem_natom_in_number_order:
            step += " "
            step += os.path.join(self.config.get_pseudo_pot_dir()["vasp"], "PAW_PBE/%s/POTCAR" % item[0])
        step += " >POTCAR\n"
        self.job.append_step(step)

        # scf        
        step = ""
        self.set_param("ICHARG", 0)
        self.set_param("LORBIT", 11)
        self.kpoints.set_kpoints([3, 3, 3, 0, 0, 0], "automatic", self.kpath)
        step += "cat >INCAR<<EOF\n"
        step += self.incar.to_string()
        step += "EOF\n"
        step += "cat >KPOINTS<<EOF\n"
        step += self.kpoints.to_string()
        step += "EOF\n"
        step += "cat >POSCAR<<EOF\n"
        step += self.poscar.to_string("cartesian")
        step += "EOF\n"
        step += "$CMD_HEAD %s\n" % self.job.run_params["cmd"]
        step += "cp vasprun.xml vasprun.scf.xml\n"
        self.job.append_step(step)

        # nscf
        step = ""
        self.set_param("ICHARG", 11)
        self.kpoints.set_kpoints([5, 5, 5, 0, 0, 0], "automatic", self.kpath)
        step += "cat >INCAR<<EOF\n"
        step += self.incar.to_string()
        step += "EOF\n"
        step += "cat >KPOINTS<<EOF\n"
        step += self.kpoints.to_string()
        step += "EOF\n"
        step += "cat >POSCAR<<EOF\n"
        step += self.poscar.to_string("cartesian")
        step += "EOF\n"
        step += "$CMD_HEAD %s\n" % self.job.run_params["cmd"]
        step += "cp vasprun.xml vasprun.nscf.xml\n"
        self.job.append_step(step)

        # bands
        step = ""
        step += "cat >INCAR<<EOF\n"
        self.kpoints.set_kpoints([3, 3, 3, 0, 0, 0], "bands", self.kpath)
        self.set_param("ICHARG", 11)
        step += self.incar.to_string()
        step += "EOF\n"
        step += "cat >KPOINTS<<EOF\n"
        step += self.kpoints.to_string()
        step += "EOF\n"
        step += "$CMD_HEAD %s\n" % self.job.run_params["cmd"]
        step += "cp vasprun.xml vasprun.bands.xml\n"
        self.job.append_step(step)

        self.job.run(directory)
        
class Dos(Vasp):
    def __init__(self):
        super().__init__()

    def run(self, directory):
        import os

        self.set_param("IBRION", -1)

        step = ""
        step += "cd ${ABSOLUTE_WORK_DIR}\n"
        step += "cat"
        for item in self.poscar.elem_natom_in_number_order:
            step += " "
            step += os.path.join(self.config.get_pseudo_pot_dir()["vasp"], "PAW_PBE/%s/POTCAR" % item[0])
        step += " >POTCAR\n"
        self.job.append_step(step)

        # scf        
        step = ""
        self.set_param("ICHARG", 0)
        self.set_param("LORBIT", 11)
        self.kpoints.set_kpoints([3, 3, 3, 0, 0, 0], "automatic", Kpath())
        step += "cat >INCAR<<EOF\n"
        step += self.incar.to_string()
        step += "EOF\n"
        step += "cat >KPOINTS<<EOF\n"
        step += self.kpoints.to_string()
        step += "EOF\n"
        step += "cat >POSCAR<<EOF\n"
        step += self.poscar.to_string("cartesian")
        step += "EOF\n"
        step += "$CMD_HEAD %s\n" % self.job.run_params["cmd"]
        step += "cp vasprun.xml vasprun.scf.xml\n"
        self.job.append_step(step)

        # nscf + dos
        step = ""
        self.set_param("ICHARG", 11)
        self.set_param("LORBIT", 11)
        self.kpoints.set_kpoints([5, 5, 5, 0, 0, 0], "automatic", Kpath())
        step += "cat >INCAR<<EOF\n"
        step += self.incar.to_string()
        step += "EOF\n"
        step += "cat >KPOINTS<<EOF\n"
        step += self.kpoints.to_string()
        step += "EOF\n"
        step += "cat >POSCAR<<EOF\n"
        step += self.poscar.to_string("cartesian")
        step += "EOF\n"
        step += "$CMD_HEAD %s\n" % self.job.run_params["cmd"]
        step += "cp vasprun.xml vasprun.nscf.dos.xml\n"
        self.job.append_step(step)

        self.job.run(directory)