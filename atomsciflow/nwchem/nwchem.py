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

from atomsciflow.cpp import nwchem
from atomsciflow.cpp.server import JobScheduler

class NWChem(nwchem.NWChem):
    def __init__(self):
        super().__init__()

class Static(NWChem):
    def __init__(self):
        super().__init__()
        self.add_keyword("task[0]", "dft", "energy")
        self.set_simple("task[0]", True)

class Opt(NWChem):
    def __init__(self):
        super().__init__()
        self.add_keyword("task[0]", "dft", "optimize")
        self.set_simple("task[0]", True)

class MD(NWChem):
    def __init__(self):
        super().__init__()
        self.add_keyword("task[0]", "dft", "qmd")
        self.set_simple("task[0]", True)

class Freq(NWChem):
    def __init__(self):
        super().__init__()
        self.set_keywords("task[0]", ["dft", "freq"])

class Neb(NWChem):
    def __init__(self):
        super().__init__()
        self.set_keywords("task[0]", ["neb", "ignore"])

    def set_neb_images(self, images, mode = 0):
        self.neb_images = images
        self.neb_images_mode = mode

    def run(self, directory):
        self.set_status("geometry[0]", False)

        from atomsciflow.cpp.base import Xyz
        xyz_images = []
        for image in self.neb_images:
            xyz = Xyz()
            xyz.read_xyz_file(image)
            xyz_images.append(xyz)
        # TODO: remove this
        self.job.set_run("xyz_file", self.neb_images[0])

        # there are three ways to set up the initial path
        if self.neb_images_mode == 0:
            # 0 -> specifying two geometries
            self.set_keywords("geometry@0[0]", ["nocenter", "noautosym", "noautoz", "units", "angstroms"])
            i = 0
            for item in xyz_images[0].atoms:
                self.set_field("geometry@0[0]", item.name, i, 0)
                self.set_field("geometry@0[0]", item.x, i, 1)
                self.set_field("geometry@0[0]", item.y, i, 2)
                self.set_field("geometry@0[0]", item.z, i, 3)
                i = i +1
            self.set_keywords("geometry@1[0]", ["endgeom", "nocenter", "noautosym", "noautoz", "units", "angstroms"])
            i = 0
            for item in xyz_images[1].atoms:
                self.set_field("geometry@1[0]", item.name, i, 0)
                self.set_field("geometry@1[0]", item.x, i, 1)
                self.set_field("geometry@1[0]", item.y, i, 2)
                self.set_field("geometry@1[0]", item.z, i, 3)
                i = i +1
        elif self.neb_images_mode == 1:
            # 1 -> specifying three geometries
            self.set_keywords("geometry@0[0]", ["nocenter", "noautosym", "noautoz", "units", "angstroms"])
            i = 0
            for item in xyz_images[0].atoms:
                self.set_field("geometry@0[0]", item.name, i, 0)
                self.set_field("geometry@0[0]", item.x, i, 1)
                self.set_field("geometry@0[0]", item.y, i, 2)
                self.set_field("geometry@0[0]", item.z, i, 3)        
                i = i +1
            self.set_keywords("geometry@1[0]", ["midgeom", "nocenter", "noautosym", "noautoz", "units", "angstroms"])
            i = 0
            for item in xyz_images[1].atoms:
                self.set_field("geometry@1[0]", item.name, i, 0)
                self.set_field("geometry@1[0]", item.x, i, 1)
                self.set_field("geometry@1[0]", item.y, i, 2)
                self.set_field("geometry@1[0]", item.z, i, 3)        
                i = i +1
            self.set_keywords("geometry@2[0]", ["endgeom", "nocenter", "noautosym", "noautoz", "units", "angstroms"])
            i = 0
            for item in xyz_images[2].atoms:
                self.set_field("geometry@2[0]", item.name, i, 0)
                self.set_field("geometry@2[0]", item.x, i, 1)
                self.set_field("geometry@2[0]", item.y, i, 2)
                self.set_field("geometry@2[0]", item.z, i, 3)
                i = i +1
        elif self.neb_images_mode == 2:
            import os
            # 2 -> specifying path through neb[0]/xyz_path
            self.set_keywords("neb[0]/xyz_path", [os.path.join(os.path.abspath(directory), "neb-path.xyz")])
        self.xyz.read_xyz_file(self.neb_images[0])
        for item in self.xyz.elements_set:
            self.set_keywords(
                "basis[0]/%s" % item,
                ["library", "6-31g"]
            )
            self.set_simple(
                "basis[0]/%s" % item,
                True
            )

        step = ""
        step += "cd ${ABSOLUTE_WORK_DIR}\n"
        step += "cat >neb-path.xyz<<EOF\n"
        for image in self.neb_images:
            with open(image, "r") as fin:
                lines = fin.readlines()
            for line in lines:
                line.replace("\n", "")
                if line.isspace():
                    continue
                step += line
        step += "\n"
        step += "EOF\n"
        self.job.append_step(step)
        step = ""

        step = ""
        step += "cat > nwchem.nw<<EOF\n"
        step += self.to_string()
        step += "EOF\n"
        step += "$CMD_HEAD %s nwchem.nw > nwchem.out\n" % self.job.run_params["cmd"]
        self.job.append_step(step)
        step = ""

        self.job.run(directory)
