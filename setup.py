#!/usr/bin/env python
# _*_ coding: utf-8 _*_

import os
import sys
import re
import subprocess
from glob import glob

import setuptools
#from setuptools import setup
from skbuild import setup  # replace setuptools setup
from setuptools import find_packages, Extension
from setuptools import Command
from setuptools.command.build_ext import build_ext

class CleanCommand(Command):
    """Custom clean command to tidy up the project root."""
    user_options = []
    def initialize_options(self):
        pass
    def finalize_options(self):
        pass
    def run(self):
        os.system('rm -vrf ./build ./dist ./*.pyc ./*.tgz ./*.egg-info')
        os.system("rm -rf _skbuild")



# -----------------------------
# make sure pybind11 is working
# -----------------------------
try:
    import pybind11
except:
    subprocess.run(["pip3", "install", "--user", "pybind11[global]==2.7.1"])
os.environ["PATH"] = os.environ["PATH"] + ":%s" % os.path.join(os.path.expanduser("~"), ".local/bin")
sub = subprocess.run(["pybind11-config", "--includes"], stdout=subprocess.PIPE)
pybind11_include_paths = sub.stdout.decode().replace("\n", "")
# there might be more than one dir in output of pybind11-config --includes
list_paths = pybind11_include_paths.replace(" ", "").split("-I")
while "" in list_paths:
    list_paths.remove("")
if "CPLUS_INCLUDE_PATH" not in os.environ:
    os.environ["CPLUS_INCLUDE_PATH"] = ":".join(list_paths)
else:
    os.environ["CPLUS_INCLUDE_PATH"] = os.environ["CPLUS_INCLUDE_PATH"] + ":%s" % ":".join(list_paths)

setup(
    name = "atomsciflow",
    version = '0.0.0',
    ## python3 setup.py build sdist bdist_wheel
    ## twine upload dist/*
    keywords = ("Atom Science, Workflow"),
    description = "A workflow manger for scientific research involving atoms",
    license = "MIT",
    url = "https://gitlab.com/deqitang/atomsciflow",
    author_email = "deqi_tang@163.com",
    packages = find_packages(),
    #data_files
    include_package_data = True,
    platforms = "any",
    python_requires = ">=3.0",
    install_requires = ["pybind11", "numpy", "scipy", "matplotlib"],
    cmdclass = {
        'clean': CleanCommand,
        'build_ext': build_ext 
    },    
    ext_modules=[
    ],
    # ------------------------------------------
    # cpp extension using scikit-build (working)
    # ------------------------------------------
    cmake_source_dir="pybind11", # where CMakeLists.txt exists
    cmake_install_dir="atomsciflow/cpp", # from atomsciflow.cpp import xxx 
    cmake_args=["-DCMAKE_BUILD_TYPE=Debug"],
    # ------------------------------------------
    scripts = [
    ],
    entry_points = {
        'console_scripts': [
            'atomsciflow = atomsciflow.cmd.atomsciflow:main',
        ]
    },
)

