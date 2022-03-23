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

def add_cp2k_subparser(subparsers):
    subparser = subparsers.add_parser("cp2k", 
        help="The CP2K calculator")

    subparser.add_argument("-d", "--directory", type=str, default="atomsciflow-calc-running-dir",
        help="The working directory where calculation is happening")

    ag = subparser.add_argument_group(title="Structure", description="Specification of structure files")

    ag.add_argument("--xyz", type=str, default=None, required=True,
        help="Specify the xyz structure file")

def cp2k_processor(args):

    from atomsciflow.cpp.cp2k import Cp2k
    job = Cp2k()
    job.get_xyz(args.xyz)
    job.run("atomsciflow-running-dir")
    