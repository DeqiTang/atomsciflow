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

from atomsciflow.cpp import elk
from atomsciflow.cpp.server import JobScheduler

class Elk(elk.Elk):
    def __init__(self):
        super().__init__()
        self.set_param("spinpol", ".true.")
        self.set_param("epspot", 1.0e-6)
        self.set_param("ngridk", [3, 3, 3])
        self.set_param("mixtype", 3)
        self.set_param("xctype", 20)
        self.set_param("lmaxapw", 8)
        self.set_param("xctype", 20)
        self.set_param("stype", 3)
        self.set_param("mixtype", 3)
        self.set_param("epspot", 1.0e-6)
        self.set_param("epsengy", 1.0e-5)

class Static(Elk):
    def __init__(self):
        super().__init__()

class Opt(Elk):
    def __init__(self):
        super().__init__()
        self.set_param("tasks", 2)
        self.set_param("latvopt", 0)

class VcOpt(Elk):
    def __init__(self):
        super().__init__()
        self.set_param("tasks", 2)
        self.set_param("latvopt", 1)

class Band(Elk):
    def __init__(self):
        super().__init__()

class Phonon(Elk):
    def __init__(self):
        super().__init__()

class Dos(Elk):
    def __init__(self):
        super().__init__()

class Phonopy(Elk):
    def __init__(self):
        super().__init__()        
        self.job.set_run("phonopy_dim_x", 1)
        self.job.set_run("phonopy_dim_y", 1)
        self.job.set_run("phonopy_dim_z", 1)
        
        # set tforce = .true. to calculate the force at end of SCF
        # so that in the post-processing, the 'FORCE_SETS' file can be
        # created successfully
        self.set_param("tforce", ".ture.")

        self.set_param("spinpol", ".false.")
        self.set_param("epspot", 1.0e-6)
        self.set_param("ngridk", [3, 3, 3])
        self.set_param("mixtype", 3)
        self.set_param("xctype", 20)
        self.set_param("lmaxapw", 8)
        self.set_param("xctype", 20)
        self.set_param("stype", 3)
        self.set_param("mixtype", 3)
        self.set_param("epspot", 1.0e-6)
        self.set_param("epsengy", 1.0e-5)

    def run(self, directory):
        step = "cd ${ABSOLUTE_WORK_DIR}\n"
        step += "cat >elk.in<<EOF\n"
        step += self.to_string()
        step += "EOF\n"
        self.job.append_step(step)

        step = "cat >elk-input-part-1.in<<EOF\n"
        self.set_block_status("atoms", False)
        self.set_block_status("avec", False)
        step += self.to_string()
        step += "EOF\n"
        self.job.append_step(step)

        step = "phonopy --elk -c %s -d --dim='%s %s %s'" % (
            "elk.in",
            self.job.run_params["phonopy_dim_x"],
            self.job.run_params["phonopy_dim_y"],
            self.job.run_params["phonopy_dim_z"]
        )
        self.job.append_step(step)

        step = "for item in supercell-*.in\n"
        step += "do\n"
        step += "mkdir run-${item/.in/}\n"
        step += "cat elk-input-part-1.in ${item} > run-${item/.in/}/elk.in\n"
        step += "done\n"
        self.job.append_step(step)

        step = "for item in run-supercell-*\n"
        step += "do\n"
        step += "cd ${item}\n"
        step += "$CMD_HEAD %s elk.in > elk.out\n" % (
            self.job.run_params["cmd"]
        )
        step += "cd ../\n"
        step += "done\n"
        self.job.append_step(step)

        self.job.run(directory)