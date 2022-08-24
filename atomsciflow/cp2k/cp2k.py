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
        self.set_param("force_eval/dft/mgrid/cutoff", 300)
        self.set_param("force_eval/dft/mgrid/rel_cutoff", 100)
        self.set_param("force_eval/dft/mgrid/ngrids", 4)
        self.set_param("force_eval/dft/qs/eps_default", 1.0e-14)
        self.set_param("force_eval/dft/scf/eps_scf", 1.0e-7)
        self.set_param("force_eval/dft/scf/max_scf", 50)
        self.set_param("force_eval/dft/qs/extrapolation", "ASPC")
        self.set_param("force_eval/method", "QS")

class Static(cp2k.Static):
    def __init__(self):
        super().__init__()
        self.set_param("global/run_type", "ENERGY_FORCE")
        self.set_param("force_eval/dft/mgrid/cutoff", 300)
        self.set_param("force_eval/dft/mgrid/rel_cutoff", 100)
        self.set_param("force_eval/dft/mgrid/ngrids", 4)
        self.set_param("force_eval/dft/qs/eps_default", 1.0e-14)
        self.set_param("force_eval/dft/scf/eps_scf", 1.0e-7)
        self.set_param("force_eval/dft/scf/max_scf", 50)  
        self.set_param("force_eval/dft/qs/extrapolation", "ASPC")      
        self.set_param("force_eval/method", "QS")
        # dos
        self.set_param("force_eval/dft/print/pdos/nlumo", -1)
        self.set_param("force_eval/dft/print/pdos/components", ".true.")
        # band
        self.set_param("force_eval/dft/print/band_structure/file_name", "bands.bs")
        self.set_param("force_eval/dft/print/band_structure/added_mos", 10)
        # Miscellaneous
        self.set_param("force_eval/dft/print/mo_cubes/nlumo", 5)
        self.set_param("force_eval/dft/print/mo_cubes/nhomo", 5)
        self.set_param("force_eval/dft/print/mo_cubes/write_cube", ".true.")
        self.set_param("force_eval/dft/print/mo/eigenvalues", ".TRUE.")
        self.set_param("force_eval/dft/print/mo/occupation_numbers", ".TRUE.")

class Band(Cp2k):
    def __init__(self):
        super().__init__()
        self.set_param("global/run_type", "ENERGY_FORCE")
        self.set_param("force_eval/dft/print/band_structure/file_name", "bands.bs")
        # to output the Fermi energy and HOMO-LUMO gap, set force_eval/dft/print/mo_cubes
        self.set_param("force_eval/dft/print/mo_cubes/nlumo", 5)
        self.set_param("force_eval/dft/print/mo_cubes/nhomo", 5)
        self.set_param("force_eval/dft/print/mo_cubes/write_cube", ".false.")
        self.set_param("force_eval/dft/print/band_structure/added_mos", 10)


class Dos(Cp2k):
    def __init__(self):
        super().__init__()
        self.set_param("global/run_type", "ENERGY_FORCE")
        self.set_param("force_eval/dft/print/pdos/nlumo", -1)
        # if FORCE_EVAL/DFT/PRINT/PDOS/COMPONENTS is set to .FALSE.(dfault)
        # the pdos will be print out, projected to angular momentum l components(e.g. s p d f).
        # when it is set to .TRUE. the pdos will be projected to angular momentum l
        # and magnetic quantum number m components(e.g. px py pz)
        self.set_param("force_eval/dft/print/pdos/components", ".true.")

class Scf(Cp2k):
    def __init__(self):
        super().__init__()
        self.set_param("global/run_type", "ENERGY_FORCE")

class Opt(Cp2k):
    def __init__(self):
        super().__init__()
        self.set_param("global/run_type", "geo_opt")
        self.set_param("global/project", "optimization")
        self.set_param("force_eval/dft/scf/eps_scf", 1.0e-7)
        self.set_param("force_eval/dft/scf/max_scf", 50)
        self.set_param("motion/geo_opt/max_iter", 1000)
        self.set_param("motion/geo_opt/optimizer", "BFGS")
        self.set_param("motion/geo_opt/rms_force", 1.0e-4)
        self.set_param("motion/geo_opt/rms_dr", 1.0e-3)
        self.set_param("motion/print/cell/each/geo_opt", 1)

class VcOpt(Cp2k):
    def __init__(self):
        super().__init__()
        self.set_param("global/run_type", "CELL_OPT")
        self.set_param("global/project", "cell-optimization")
        self.set_param("global/print_level", "LOW")
        self.set_param("force_eval/stress_tensor", "ANALYTICAL")
        self.set_param("motion/cell_opt/type", "DIRECT_CELL_OPT")
        self.set_param("motion/cell_opt/pressure_tolerance", 5)
        self.set_param("motion/cell_opt/keep_angles", ".FALSE.")
        self.set_param("motion/cell_opt/keep_symmetry", ".FALSE.")
        self.set_param("motion/cell_opt/max_iter", 1000)
        self.set_param("motion/cell_opt/optimizer", "BFGS")
        self.set_param("motion/cell_opt/rms_dr", 1.0e-3)
        self.set_param("motion/cell_opt/rms_force", 1.0e-4)
        self.set_param("motion/geo_opt/type", "MINIMIZATION")
        self.set_param("motion/geo_opt/max_iter", 1000)
        self.set_param("motion/geo_opt/optimizer", "BFGS")
        self.set_param("motion/geo_opt/rms_dr", 1.0e-3)     
        self.set_param("motion/geo_opt/rms_force", 1.0e-4)
        self.set_param("motion/print/trajectory/each/cell_opt", 1)
        self.set_param("motion/print/cell/each/geo_opt", 1)

class Vib(Cp2k):
    def __init__(self):
        super().__init__()
        self.set_param("global/run_type", "VIBRATIONAL_ANALYSIS")
        self.set_param("vibrational_analysis/print/molden_vib(on)/each/replica_eval", 1)
        self.set_param("vibrational_analysis/dx", 0.01)

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

        self.plumed.set_param("d1:distance/atoms", [1, 2])
        self.plumed.set_param(":print/arg", "d1")
        self.plumed.set_param(":print/file", "colvar.data")

    def run(self, directory):
        step = ""
        step += "cd ${ABSOLUTE_WORK_DIR}\n"
        step += "cat >%s<<EOF\n" % self.job.run_params["input"]
        step += self.to_string()
        step += "EOF\n"
        step += "cat >plumed.dat<<EOF\n"
        step += self.plumed.to_string()
        step += "EOF\n"
        step += "$CMD_HEAD %s -in %s | tee %s  \n" % (
            self.job.run_params["cmd"],
            self.job.run_params["input"],
            self.job.run_params["output"]
        )
        self.job.append_step(step)

        self.job.run(directory)

class Phonopy(Cp2k):
    def __init__(self):
        super().__init__()
        self.job.set_run("phonopy_dim_x", 1)
        self.job.set_run("phonopy_dim_y", 1)
        self.job.set_run("phonopy_dim_z", 1)
        self.set_param("force_eval/stress_tensor", "ANALYTICAL")
        self.set_param("force_eval/dft/mgrid/cutoff", 300)
        self.set_param("force_eval/dft/mgrid/rel_cutoff", 100)
        self.set_param("force_eval/dft/qs/eps_default", 1.0e-14)
        self.set_param("force_eval/dft/scf/eps_scf", 1.0e-7)
        self.set_param("force_eval/dft/scf/max_scf", 50)

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

class Neb(cp2k.Neb):
    def __init__(self):
        super().__init__()
        self.set_param("force_eval/dft/mgrid/cutoff", 300)
        self.set_param("force_eval/dft/mgrid/rel_cutoff", 100)
        self.set_param("force_eval/dft/mgrid/ngrids", 4)
        self.set_param("force_eval/dft/qs/eps_default", 1.0e-14)
        self.set_param("force_eval/dft/scf/eps_scf", 1.0e-7)
        self.set_param("force_eval/dft/scf/max_scf", 50)
        self.set_param("force_eval/dft/qs/extrapolation", "ASPC")
        self.set_param("force_eval/method", "QS")
        #        
        self.set_param("motion/band/band_type", "CI-NEB")
        self.set_param("motion/band/k_spring", 2.0e-2)
        self.set_param("motion/band/rotate_frames", "TRUE")
        self.set_param("motion/band/convergence_info(on)/each/band", 1)
        self.set_param("motion/band/energy(on)/each/band", 1)
        self.set_param("motion/band/optimize_band/diis/max_steps", 120)
        self.set_param("motion/band/program_run_info/initial_configuration_info", "TRUE")
        self.set_param("motion/print/trajectory/each/band", 1)
