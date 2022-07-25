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

def add_cp2k_post_subparser(subparsers):
    subparser = subparsers.add_parser("cp2k", 
        help="The CP2K post-processor")

    subparser.add_argument("-d", "--directory", type=str, default="atomsciflow-calc-running-dir",
        help="The working directory where calculation is happening")

    subparser.add_argument("-c", "--calc", type=str, default="static",
        choices=["static", "opt", "vcopt", "vib", "md", "metamd", "phonopy", "dos", "band"],
        help="The calculation to do. The specified value is case insensitive")
        
    ag = subparser.add_argument_group(title="kpoints")

    ag.add_argument("--kpath", type=str, default=None,
        help="Specify the kpath, either from a file or command line string, e.g. --kpath kpath.txt or --kpath \"0 0 0 GAMMA 5;0.5 0 0 x |\"")


def cp2k_post_processor(args):
    print("working directory: %s" % args.directory)
    if args.calc.lower() == "static":
        pass
    elif args.calc.lower() == "dos":
        from atomsciflow.cp2k.post import Pdos
        job = Pdos()
        job.run(args.directory)
    elif args.calc.lower() == "band":
        from atomsciflow.cp2k.post import Bands
        job = Bands()      
        job.run(args.directory)
    elif args.calc.lower() == "opt":
        from atomsciflow.cp2k.post import Opt
        job = Opt()
        job.run(args.directory)
    elif args.calc.lower() == "vcopt":
        from atomsciflow.cp2k.post import VcOpt
        job = VcOpt()
        job.run(args.directory)        
    elif args.calc.lower() == "vib":
        pass
    elif args.calc.lower() == "md":
        from atomsciflow.cp2k.post import MD
        job = MD()
        job.run(args.directory)
    elif args.calc.lower() == "metamd":
        pass
    elif args.calc.lower() == "phonopy":
        from atomsciflow.cp2k.post import Phonopy
        job = Phonopy()
        from atomsciflow.cpp.base import Kpath
        kpath = Kpath()
        if args.kpath.count(";") != 0:
            kpath.read(args.kpath)
        else:
            kpath.read_file(args.kpath)
        job.set_kpath(kpath)
        job.run(args.directory)
        job.extract_data(args.directory)
    else:
        print("The specified post-processing type is unfound!")
        sys.exit(1)
    