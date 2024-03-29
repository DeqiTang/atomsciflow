#!/usr/bin/env python
# _*_ coding: utf-8 _*_

import os
import sys
import re
import shutil
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

# There is an example of using pybind11 in cmake based system, providing
# full control over the cmake run. see https://github.com/pybind/cmake_example
# for more information, which is under the BSD-style license.
# Here we improved it so that it can support the building of pybind11 and cython
# based extenstion at the same time.

# Convert distutils Windows platform specifiers to CMake -A arguments
PLAT_TO_CMAKE = {
    "win32": "Win32",
    "win-amd64": "x64",
    "win-arm32": "ARM",
    "win-arm64": "ARM64",
}

# A CMakeExtension needs a sourcedir instead of a file list.
# The name must be the _single_ output extension from the CMake build.
# If you need multiple extensions, see scikit-build.
class CMakeExtension(Extension):
    def __init__(self, name, sourcedir=""):
        super().__init__(name, sources=[])
        self.sourcedir = os.path.abspath(sourcedir)

def pybind11_build_ext(builder, ext):
    extdir = os.path.abspath(os.path.dirname(builder.get_ext_fullpath(ext.name)))
    #extdir = os.path.join(extdir, "atomsciflow/cpp")
    # required for auto-detection & inclusion of auxiliary "native" libs
    if not extdir.endswith(os.path.sep):
        extdir += os.path.sep
    debug = int(os.environ.get("DEBUG", 0)) if builder.debug is None else builder.debug
    cfg = "Debug" if debug else "Release"
    # CMake lets you override the generator - we need to check this.
    # Can be set with Conda-Build, for example.
    cmake_generator = os.environ.get("CMAKE_GENERATOR", "")
    # Set Python_EXECUTABLE instead if you use PYBIND11_FINDPYTHON
    # EXAMPLE_VERSION_INFO shows you how to pass a value into the C++ code
    # from Python.
    cmake_args = [
        "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={}".format(extdir),
        "-DPYTHON_EXECUTABLE={}".format(sys.executable),
        "-DCMAKE_BUILD_TYPE={}".format(cfg),  # not used on MSVC, but no harm
    ]
    build_args = []
    # Adding CMake arguments set as environment variable
    # (needed e.g. to build for ARM OSx on conda-forge)
    if "CMAKE_ARGS" in os.environ:
        cmake_args += [item for item in os.environ["CMAKE_ARGS"].split(" ") if item]
    # In this example, we pass in the version to C++. You might not need to.
    cmake_args += [
        "-DEXAMPLE_VERSION_INFO={}".format(builder.distribution.get_version())
    ]
    if builder.compiler.compiler_type != "msvc":
        # Using Ninja-build since it a) is available as a wheel and b)
        # multithreads automatically. MSVC would require all variables be
        # exported for Ninja to pick it up, which is a little tricky to do.
        # Users can override the generator with CMAKE_GENERATOR in CMake
        # 3.15+.
        if not cmake_generator:
            try:
                import ninja  # noqa: F401

                cmake_args += ["-GNinja"]
            except ImportError:
                pass
    else:
        # Single config generators are handled "normally"
        single_config = any(x in cmake_generator for x in {"NMake", "Ninja"})
        # CMake allows an arch-in-generator style for backward compatibility
        contains_arch = any(x in cmake_generator for x in {"ARM", "Win64"})
        # Specify the arch if using MSVC generator, but only if it doesn't
        # contain a backward-compatibility arch spec already in the
        # generator name.
        if not single_config and not contains_arch:
            cmake_args += ["-A", PLAT_TO_CMAKE[builder.plat_name]]
        # Multi-config generators have a different way to specify configs
        if not single_config:
            cmake_args += [
                "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}".format(cfg.upper(), extdir)
            ]
            build_args += ["--config", cfg]
    if sys.platform.startswith("darwin"):
        # Cross-compile support for macOS - respect ARCHFLAGS if set
        archs = re.findall(r"-arch (\S+)", os.environ.get("ARCHFLAGS", ""))
        if archs:
            cmake_args += ["-DCMAKE_OSX_ARCHITECTURES={}".format(";".join(archs))]
    # Set CMAKE_BUILD_PARALLEL_LEVEL to control the parallel build level
    # across all generators.
    if "CMAKE_BUILD_PARALLEL_LEVEL" not in os.environ:
        # self.parallel is a Python 3 only way to set parallel jobs by hand
        # using -j in the build_ext call, not supported by pip or PyPA-build.
        if hasattr(builder, "parallel") and builder.parallel:
            # CMake 3.12+ only.
            build_args += ["-j{}".format(builder.parallel)]
    if not os.path.exists(builder.build_temp):
        os.makedirs(builder.build_temp)
    subprocess.check_call(
        ["cmake", ext.sourcedir] + cmake_args, cwd=builder.build_temp
    )
    subprocess.check_call(
        ["cmake", "--build", "."] + build_args, cwd=builder.build_temp
    )
    #

# -----------------------------
# make sure pybind11 is working
# -----------------------------
try:
    import pybind11
    # if there is a pybind11 directory locally in this project, it can be imported.
    # but that's not the pybind11 package we want
    # so check pybind11 package validity further
    if os.path.exists("pybind11"):
        # see if the truly pybind11 package is installed
        os.environ["PATH"] = os.environ["PATH"] + ":%s" % os.path.join(os.path.expanduser("~"), ".local/bin")
        pybind11.get_include()
    print("using pybind11 version: ", pybind11.version_info)
except:
    subprocess.check_call([sys.executable, "-m", "pip", "install", "pybind11[global]"])
    print("using newly installed pybind11 version: ", pybind11.version_info)

# --------------------------------
# fortran support
# --------------------------------
from Cython.Build import cythonize
ext_modules_fortran = cythonize(
    [
        Extension(
            'fortran.cube_handle',
            sources=[
                "fortran/pyx/cube_handle.pyx"
            ],
            # other compile args for gcc
            extra_compile_args=[
                #'-fPIC', 
                '-O3', 
                "-fopenmp" # important for OpenMP dependency
            ],
            # other files to link to
            extra_link_args=[
                'fortran/atomsciflowf/build/lib/libatomsciflowf-c-binding.a',
                'fortran/atomsciflowf/build/lib/libatomsciflowf.a',
                "-lgfortran",
            ],
            language="c"
        ),
        Extension(
            'fortran.cmd_utils',
            sources=[
                "fortran/pyx/cmd_utils.pyx"
            ],
            # other compile args for gcc
            extra_compile_args=[
                #'-fPIC', 
                '-O3',
                "-fopenmp" # important for OpenMP dependency
            ],
            # other files to link to
            extra_link_args=[
                'fortran/atomsciflowf/build/lib/libatomsciflowf-c-binding.a',
                'fortran/atomsciflowf/build/lib/libatomsciflowf.a',
                "-lgfortran",
            ],
            language="c"
        ),
    ],
    language_level="3"
)

ext_modules_all = []
ext_modules_all += ext_modules_fortran

ext_modules_fortran_names = []
for ext in ext_modules_fortran:
    ext_modules_fortran_names.append(ext.name)

def fortran_build_ext(builder, ext):
    builder.build_extension(ext)
    # we no longer need to copy the built extension.so to desired position
    # combine with ext_package of setup() and extension name specification in Extension
    # this is automatically handled.
    #os.system("mv _skbuild/linux-x86_64-*/setuptools/lib.linux-x86_64-*/%s.*.so _skbuild/linux-x86_64-*/setuptools/lib.linux-x86_64-*/atomsciflow/fortran/" % ext.name)

def build_atomsciflowf():
    atomsciflowf_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "fortran/atomsciflowf")
    shutil.rmtree(
        os.path.join(atomsciflowf_dir, "build"),
        ignore_errors=True
    ) # in case there is previous build in a different environment
    os.system("cmake %s -B %s" % (
        atomsciflowf_dir,
        os.path.join(atomsciflowf_dir, "build")
    ))
    os.system("cmake --build %s" % os.path.join(atomsciflowf_dir, "build"))

# deciding whether to build fortran extensions
if "ATOMSCIFLOW_BUILD_PY_EXT_NO_FORTRAN" in os.environ:
    if os.environ["ATOMSCIFLOW_BUILD_PY_EXT_NO_FORTRAN"].lower() == "true":
        build_py_ext_no_fortran = True
    else:
        build_py_ext_no_fortran = False
else:
    # default is to build fortran extensions
    build_py_ext_no_fortran = False

class CustomBuildExt(build_ext):
    """
    """
    def run(self):
        if False == build_py_ext_no_fortran:
            build_atomsciflowf()
        super().run()

    def build_extension(self, ext):
        if ext.name in ext_modules_fortran_names:
            fortran_build_ext(super(), ext)
        else:
            pybind11_build_ext(self, ext)

setup(
    name = "atomsciflow",
    version = '0.0.2a1',
    ## python3 setup.py build sdist bdist_wheel
    keywords = ("Atom Science, Workflow"),
    description = "A workflow manger for scientific research involving atoms",
    license = "MIT",
    url = "https://github.com/deqitang/atomsciflow",
    author_email = "deqitang@gmail.com",
    packages = find_packages(),
    #data_files
    include_package_data = True,
    platforms = "any",
    python_requires = ">=3.0",
    install_requires = ["pybind11", "numpy", "scipy"],
    cmdclass = {
        'clean': CleanCommand,
        'build_ext': CustomBuildExt
    },
    # ext_package is used as the base package import path for Extention
    # in ext_modules. i.e. if the Extension.name is set to fortran.cmd_utils
    # while ext_package is set to atomsciflow, the import path for cmd_utils
    # would be import atomsciflow.fortran.cmd_utils
    ext_package="atomsciflow",
    ext_modules=[
        # --------------------------------------------
        # cpp extension using CustomBuildExt (working)
        # --------------------------------------------
        #CMakeExtension(name="cpptest", sourcedir="cpp/pybind11")
        # --------------------------------------------
    ] + (ext_modules_fortran if build_py_ext_no_fortran == False else []),#+ext_modules_fortran,
    # ------------------------------------------
    # cpp extension using scikit-build (working)
    # ------------------------------------------
    cmake_source_dir="pybind11", # where CMakeLists.txt exists
    cmake_install_dir="atomsciflow/cpp", # from atomsciflow.cpp import xxx 
    cmake_args=["-DCMAKE_BUILD_TYPE=Release"],
    # ------------------------------------------
    scripts = [
    ],
    entry_points = {
        'console_scripts': [
            'atomsciflow = atomsciflow.cmd.atomsciflow:main',
            "atomsciflow-calc = atomsciflow.cmd.atomsciflow_calc:main",
            "atomsciflow-post = atomsciflow.cmd.atomsciflow_post:main",
            "atomsciflow-stru = atomsciflow.cmd.atomsciflow_stru:main",
        ]
    },
)

