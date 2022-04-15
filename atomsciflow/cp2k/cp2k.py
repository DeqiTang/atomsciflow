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

class Cp2k(cp2k.Cp2k):
    def __init__(self):
        super().__init__()

class Static(cp2k.Cp2kStatic):
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
        self.set_param("global/print_level", "MEDIUM")

class Vib(Cp2k):
    def __init__(self):
        super().__init__()
        self.set_param("global/run_type", "VIBRATIONAL_ANALYSIS")

class MD(Cp2k):
    def __init__(self):
        super().__init__()

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

class Neb(Cp2k):
    def __init__(self):
        super().__init__()
        self.set_param("global/run_type", "BAND")
