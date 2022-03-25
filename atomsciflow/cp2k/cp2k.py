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

class Opt(cp2k.Cp2kOpt):
    def __init__(self):
        super().__init__()
        print("cp2k.Opt -> __init__ start")
        #self.sections["global"].set_param("run_type", "geo_opt")
        self.set_param("global/run_type", "geo_opt")
        self.set_param("global/project", "optimization")
        print("cp2k.Opt -> new_section -> motion/geo_opt")
        self.new_section("motion/geo_opt")
        #self.new_section("motion")
        #self.sections["motion"].add_section("geo_opt")
        
