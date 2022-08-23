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

class VcOpt(Siesta):
    def __init__(self):
        super().__init__()
        self.set_param("MD.TypeOfRun", "CG")
        self.set_param("MD.VariableCell", "true")
        # self.set_param("MD.ConstantVolume", "true")
        self.set_param("MD.ConstantVolume", "false")
        self.set_param("MD.MaxForceTol", "0.001 eV/Ang")
        self.set_param("MD.MaxStressTol", "0.01 GPa")
        self.set_param("MD.Steps", 100)
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

class Phonopy(siesta.Phonopy):
    def __init__(self):
        super().__init__()
        self.set_param("XC.functional", "GGA")
        self.set_param("XC.Authors", "PBE")
        self.set_param("DM.Tolerance", 1.0e-6)
        self.set_param("DM.MixingWeight", 0.2)
        self.set_param("DM.NumberPulay", 8)
        self.set_param("DM.AllowExtrapolation", "true")
        self.set_param("DM.UseSaveDM", "true")
        self.set_param("SolutionMethod", "diagon")
        self.set_param("MeshCutOff", "300 Ry")

class Band(Siesta):
    def __init__(self):
        super().__init__()
        self.set_param("XC.functional", "GGA")
        self.set_param("XC.Authors", "PBE")
        self.set_param("DM.Tolerance", 1.0e-6)
        self.set_param("DM.MixingWeight", 0.2)
        self.set_param("DM.NumberPulay", 8)
        self.set_param("DM.AllowExtrapolation", "true")
        self.set_param("DM.UseSaveDM", "true")
        self.set_param("SolutionMethod", "diagon")
        self.set_param("MeshCutOff", "300 Ry")

class Dos(Siesta):
    def __init__(self):
        super().__init__()
        self.set_block_data("ProjectedDensityOfStates", ["-10.0", "10.0", "0.2", "1000", "eV"], 0)
        self.set_param("XC.functional", "GGA")
        self.set_param("XC.Authors", "PBE")
        self.set_param("DM.Tolerance", 1.0e-6)
        self.set_param("DM.MixingWeight", 0.2)
        self.set_param("DM.NumberPulay", 8)
        self.set_param("DM.AllowExtrapolation", "true")
        self.set_param("DM.UseSaveDM", "true")
        self.set_param("SolutionMethod", "diagon")
        self.set_param("MeshCutOff", "300 Ry")

class Static(Siesta):
    def __init__(self):
        super().__init__()
        self.set_param("XC.functional", "GGA")
        self.set_param("XC.Authors", "PBE")
        self.set_param("DM.Tolerance", 1.0e-6)
        self.set_param("DM.MixingWeight", 0.2)
        self.set_param("DM.NumberPulay", 8)
        self.set_param("DM.AllowExtrapolation", "true")
        self.set_param("DM.UseSaveDM", "true")
        self.set_param("SolutionMethod", "diagon")
        self.set_param("MeshCutOff", "300 Ry")        
        # band: call self.set_bandlines() before run, then band is calculated
        # dos
        self.set_block_data("ProjectedDensityOfStates", ["-10.0", "10.0", "0.2", "1000", "eV"], 0)
        # Population analysis: Mulliken, Voronoi, Hirshfeld
        self.set_param("WriteMullikenPop", 3)
        self.set_param("WriteVoronoiPop", "true")
        self.set_param("WriteHirshfeldPop", "true")
        # COOP/COHP
        self.set_param("COOP.Write", "true")
        # macroscopic polarization
        self.set_block_data("PolarizationGrids", 10, 0, 0)
        self.set_block_data("PolarizationGrids", 10, 0, 1)
        self.set_block_data("PolarizationGrids", 10, 0, 2)
        self.set_block_data("PolarizationGrids", "yes", 0, 3)
        self.set_block_data("PolarizationGrids", 10, 1, 0)
        self.set_block_data("PolarizationGrids", 10, 1, 1)
        self.set_block_data("PolarizationGrids", 10, 1, 2)
        self.set_block_data("PolarizationGrids", "yes", 1, 3)
        self.set_block_data("PolarizationGrids", 10, 2, 0)
        self.set_block_data("PolarizationGrids", 10, 2, 1)
        self.set_block_data("PolarizationGrids", 10, 2, 2)
        self.set_block_data("PolarizationGrids", "yes", 2, 3)
        # optical properties
        self.set_param("OpticalCalculation", "true")
        self.set_param("Optical.Energy.Minimum", ["0", "Ry"])
        self.set_param("Optical.Energy.Maximum", ["10", "Ry"])
        self.set_param("Optical.Broaden", ["0.1", "Ry"])
        self.set_param("Optical.Scissor", ["0", "Ry"])
        self.set_block_data("Optical.Mesh", [5, 5, 5], 0)
        self.set_param("Optical.PolarizationType", "polycrystal")
        self.set_block_data("Optical.Vector", [0.0, 0.0, 1.0], 0)
