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

from atomsciflow.cpp import siesta
from atomsciflow.cpp.server import JobScheduler
from atomsciflow.cpp.config import ConfigManager

class Siesta(siesta.Siesta):
    def __init__(self):
        super().__init__()

class Opt(Siesta):
    def __init__(self):
        super().__init__()
        self.set_param("MD.TypeOfRun", "CG")
        self.set_param("MD.VariableCell", "false")
        self.set_param("MD.ConstantVolume", "true")
        self.set_param("MD.MaxForceTol", "0.001 eV/Ang")
        self.set_param("MD.MaxStressTol", "0.01 GPa")
        self.set_param("MD.Steps", 60)
        self.set_param("MD.MaxDispl", "0.2 Bohr")
        self.set_param("MD.PreconditionVariableCell", "5 Ang")
        self.set_param("WriteCoorXmol", "true")
        self.set_param("WriteMDXmol", "true")

class MD(Siesta):
    def __init__(self):
        super().__init__()
        self.set_param("MD.TypeOfRun", "Nose")
        self.set_param("MD.InitialTimeStep", 1)
        self.set_param("MD.FinalTimeStep", 1000)
        self.set_param("MD.LengthTimeStep", "1 fs")
        self.set_param("MD.InitialTemperature", "300 K")
        self.set_param("MD.TargetTemperature", "300 K")
        self.set_param("WriteCoorXmol", "true")
        self.set_param("WriteMDXmol", "true")