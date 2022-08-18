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

from atomsciflow.cpp import octopus
from atomsciflow.cpp.server import JobScheduler

class Octopus(octopus.Octopus):
    def __init__(self):
        super().__init__()

class Static(Octopus):
    def __init__(self):
        super().__init__()

class Opt(Octopus):
    def __init__(self):
        super().__init__()
        self.set_param("CalculationMode", "go")

class Band(Octopus):
    def __init__(self):
        super().__init__()

    def set_kpath(self, kpath):
        self.kpath = kpath

    def _set_kpath(self):
        for i in range(len(self.kpath.links) - 1):
            self.set_block_data("KPointsPath", self.kpath.links[i], 0, i)
        for i in range(len(self.kpath.coords)):
            self.set_block_data("KPointsPath", self.kpath.coords[i], i+1)

    def run(self, directory):
        self.set_status("Radius", False)
        
        step = ""
        self.set_param("CalculationMode", "gs")        
        step += "cd ${ABSOLUTE_WORK_DIR}\n"
        step += "cat >%s<<EOF\n" % self.job.run_params["input"]
        step += self.to_string()
        step += "EOF\n"
        step += "$CMD_HEAD %s > octopus-gs.out\n" % self.job.run_params["cmd"]
        self.job.append_step(step)

        step = ""
        self.set_param("CalculationMode", "unocc")
        self.set_param("ExtraStates", 10)
        self.set_param("ExtraStatesToConverge", 5)
        self._set_kpath()
        self.set_block_status("KPointsGrid", False)
        step += "cat >%s<<EOF\n" % self.job.run_params["input"]
        step += self.to_string()
        step += "EOF\n"
        step += "$CMD_HEAD %s > octopus-unocc.out\n" % self.job.run_params["cmd"]
        self.job.append_step(step)

        self.job.run(directory)

class Dos(Octopus):
    def __init__(self):
        super().__init__()

        self.set_status("Radius", False)
        self.set_param("CalculationMode", "gs")
        self.set_block_data("Output", "dos", 0, 0)
