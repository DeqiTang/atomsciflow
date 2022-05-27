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

from atomsciflow.cpp import cp2k
from atomsciflow.cpp.server import JobScheduler
from atomsciflow.cpp.plumed import (
    Plumed,
    Action
)
from atomsciflow.cpp.base import Kpath

class Cp2k(cp2k.Cp2k):
    def __init__(self):
        super().__init__()

class Static(cp2k.Static):
    def __init__(self):
        super().__init__()
        self.set_param("global/run_type", "ENERGY_FORCE")

class Band(cp2k.Cp2k):
    def __init__(self):
        super().__init__()
        self.set_param("global/run_type", "ENERGY_FORCE")

class Opt(cp2k.Cp2k):
    def __init__(self):
        super().__init__()
        self.set_param("global/run_type", "geo_opt")
        self.set_param("global/project", "optimization")

class VcOpt(cp2k.Cp2k):
    def __init__(self):
        super().__init__()
        self.set_param("global/run_type", "CELL_OPT")
        self.set_param("global/project", "cell-optimization")
        self.set_param("global/print_level", "LOW")
        self.set_param("force_eval/stress_tensor", "NUMERICAL")
        self.set_param("motion/geo_opt/max_iter", 100)
        self.set_param("motion/cell_opt/keep_angles", ".TRUE.")
        self.set_param("motion/cell_opt/max_iter", 100)
        self.set_param("motion/cell_opt/optimizer", "BFGS")
        self.set_param("motion/cell_opt/rms_dr", 1.0e-3)
        self.set_param("motion/cell_opt/rms_force", 1.0e-4)

class Vib(Cp2k):
    def __init__(self):
        super().__init__()
        self.set_param("global/run_type", "VIBRATIONAL_ANALYSIS")

class MD(Cp2k):
    def __init__(self):
        super().__init__()
        self.set_param("global/run_type", "MD")
        self.set_param("motion/md/ensemble", "NVT")
        self.set_param("motion/md/timestep", 0.5)
        self.set_param("motion/md/steps", 1000)
        self.set_param("motion/md/thermostat/type", "NOSE")
        self.set_param("motion/md/thermostat/region", "MASSIVE")
        self.set_param("motion/md/thermostat/nose/timecon", 10.0)
        self.set_param("motion/md/temperature", 300)

class MetaMD(Cp2k):
    def __init__(self):
        super().__init__()
        self.set_param("global/run_type", "MD")
        self.set_param("motion/md/ensemble", "NVT")
        self.set_param("motion/md/timestep", 0.5)
        self.set_param("motion/md/steps", 1000)
        self.set_param("motion/md/thermostat/type", "NOSE")
        self.set_param("motion/md/thermostat/region", "MASSIVE")
        self.set_param("motion/md/thermostat/nose/timecon", 10.0)
        self.set_param("motion/md/temperature", 300)
        self.set_param("motion/free_energy/metadyn/delta_t", 0.0)
        self.set_param("motion/free_energy/metadyn/do_hills", ".TRUE.")
        self.set_param("motion/free_energy/metadyn/nt_hills", 30)

class MetaMDPlumed(Cp2k):
    def __init__(self):
        super().__init__()
        self.set_param("global/run_type", "MD")
        self.set_param("motion/md/ensemble", "NVT")
        self.set_param("motion/md/timestep", 0.5)
        self.set_param("motion/md/steps", 1000)
        self.set_param("motion/md/thermostat/type", "NOSE")
        self.set_param("motion/md/thermostat/region", "MASSIVE")
        self.set_param("motion/md/thermostat/nose/timecon", 10.0)
        self.set_param("motion/md/temperature", 300)
        self.set_param("motion/free_energy/metadyn/use_plumed", ".TRUE.")
        self.set_param("motion/free_energy/metadyn/plumed_input_file", "./plumed.dat")

        self.plumed.add_action("d:distance")
        self.plumed.add_action(":print")
        self.plumed.actions["d:distance"].add_keyword("atoms=1,2")
        self.plumed.actions[":print"].add_keyword("arg=d")
        self.plumed.actions[":print"].add_keyword("file=colvar")

class Neb(Cp2k):
    def __init__(self):
        super().__init__()
        self.set_param("global/run_type", "BAND")

class Phonopy(Cp2k):
    def __init__(self):
        super().__init__()
        self.job.set_run("phonopy_dim_x", 1)
        self.job.set_run("phonopy_dim_y", 2)
        self.job.set_run("phonopy_dim_z", 3)
    
    def run(self, directory):
        step = "cd ${ABSOLUTE_WORK_DIR}\n"
        step += "cat >%s<<EOF\n" % self.job.run_params["input"]
        step += self.to_string()
        step += "EOF\n"
        self.job.append_step(step)

        step = "phonopy --cp2k -c %s -d --dim='%s %s %s'" % (
            self.job.run_params["input"],
            self.job.run_params["phonopy_dim_x"],
            self.job.run_params["phonopy_dim_y"],
            self.job.run_params["phonopy_dim_z"]
        )
        self.job.append_step(step)

        step = "for inp in %s-supercell-*.inp\n" % self.job.run_params["input"].split(".inp")[0]
        step += "do\n"
        step += "$CMD_HEAD %s -in %s | tee %s \n" % (
            self.job.run_params["cmd"],
            "${inp}",
            "${inp}.out"
        )
        step += "done\n"
        self.job.append_step(step)

        self.job.run(directory)
