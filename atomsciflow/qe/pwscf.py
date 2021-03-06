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
from atomsciflow.cpp.qe import PwScfMisc
from atomsciflow.cpp.base import Xyz
from atomsciflow.cpp.server import JobScheduler

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
