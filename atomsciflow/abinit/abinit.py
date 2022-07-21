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

from atomsciflow.cpp import abinit
from atomsciflow.cpp.server import JobScheduler
from atomsciflow.cpp.base import Xyz

class Abinit(abinit.Abinit):
    def __init__(self):
        super().__init__()

class Static(Abinit):
    def __init__(self):
        super().__init__()

class Opt(Abinit):
    def __init__(self):
        super().__init__()
        self.set_param("ionmov", 3)
        self.set_param("optcell[0]", 0)
        self.set_param("ntime[0]", 100)
        self.set_param("tolmxf[0]", 0)
        self.set_param("tolmxde[0]", "0.0001 eV")        

class MD(Abinit):
    def __init__(self):
        super().__init__()
        self.set_param("ionmov", 8)
        self.set_param("dtion", 100)
        self.set_param("ntime", 1000)
        self.set_param("mdtemp(1)[0]", 300)
        self.set_param("mdtemp(2)[0]", 300)
        self.set_param("tolmxf[0]", 5.0e-4)

class DfptElasticPiezoDielec(abinit.Abinit):
    def __init__(self):
        super().__init__()

        self.set_ndtset(3)

        self.set_param("tolvrs[1]", 1.0e-18)
        self.set_param("ngkpt[1]", [1, 1, 1])

        self.set_param("tolwfr[2]", 1.0e-22)
        self.set_param("getwfc[2]", -1)
        self.set_param("kptopt[2]", 2)
        self.set_param("ngkpt[2]", [1, 1, 1])
        self.set_param("iscf[2]", -3)
        self.set_param("rfelfd[2]", 2)
        self.set_param("rfdir[2]", [1, 1, 1])
        self.set_param("nqpt[2]", 1)
        self.set_param("qpt[2]", [0, 0, 0])

        self.set_param("tolvrs[3]", 1.0e-10)
        self.set_param("kptopt[3]", 2)
        self.set_param("ngkpt[3]", [1, 1, 1])
        self.set_param("getddk[3]", -1)
        self.set_param("getwfk[3]", -2)
        self.set_param("nqpt[3]", 1)
        self.set_param("qpt[3]", [0, 0, 0])
        self.set_param("rfphon[3]", 1)
        self.set_param("rfelfd[3]", 3)
        self.set_param("rfatpol[3]", [1, self.datasets[0].system.xyz.natom()])
        self.set_param("rfdir[3]", [1, 1, 1])
        self.set_param("rfstrs[3]", 3)

class Phonopy(abinit.Phonopy):
    def __init__(self):
        super().__init__()
