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
