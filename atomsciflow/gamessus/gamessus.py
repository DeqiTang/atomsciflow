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

from atomsciflow.cpp import gamessus
from atomsciflow.cpp.server import JobScheduler

class GamessUS(gamessus.GamessUS):
    def __init__(self):
        super().__init__()

class Static(gamessus.GamessUSStatic):
    def __init__(self):
        super().__init__()

class Opt(gamessus.GamessUSOpt):
    def __init__(self):
        super().__init__()
        self.set_param("guess/guess", "huckel")

class MD(GamessUS):
    def __init__(self):
        super().__init__()

        self.set_param("contrl/scftyp", "UHF")
        self.set_param("contrl/coord", "unique")
        self.set_param("contrl/mult", 1)
        self.set_param("contrl/runtyp", "md")
        self.set_param("contrl/local", "boys")

        self.set_param("system/timlim", 1)
        self.set_param("basis/gbasis", "sto")
        self.set_param("basis/ngauss", 2)

        self.set_param("guess/guess", "huckel")

        self.set_param("md/mdint", "VVERLET")
        self.set_param("md/dt", 5.0e-15)
        self.set_param("md/nvtnh", 2)
        self.set_param("md/read", ".false.")
        self.set_param("md/mbt", ".true.")
        self.set_param("md/mbr", ".true.")

class Hessian(GamessUS):
    def __init__(self):
        super().__init__()

        self.set_param("contrl/scftyp", "UHF")
        self.set_param("contrl/coord", "unique")
        self.set_param("contrl/mult", 1)
        self.set_param("contrl/runtyp", "md")
        self.set_param("contrl/local", "boys")

        self.set_param("system/timlim", 1)
        self.set_param("basis/gbasis", "sto")
        self.set_param("basis/ngauss", 2)

        self.set_param("guess/guess", "huckel")

        self.set_param("force/vibsiz", "0.01")
        self.set_param("force/temp", "298.15")
        
        self.set_param("cphf/polar", ".true.")

