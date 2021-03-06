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
