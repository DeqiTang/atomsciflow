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

from atomsciflow.cmd.calc_tools import (
    add_calc_parser_common,
    set_calc_processor_common
)

def add_abinit_subparser(subparsers):
    subparser = subparsers.add_parser("abinit", 
        help="The Abinit calculator")
    
    add_calc_parser_common(subparser)

    subparser.add_argument("-c", "--calc", type=str, default="static",
        choices=["static", "opt", "vcopt", "md", "phonopy", "dfpt-epd", "band", "dos"],
        help="The calculation to do. The specified value is case insensitive")

    subparser.add_argument("--pot", type=str, default="ncpp",
        choices=["ncpp", "paw"],
        help="Specify the pseudo potential setting.")

    # custom
    ag = subparser.add_argument_group(title="custom")
    
    ag.add_argument("--custom", type=str, default=None,
        help="Specify parameters that are not provided directly in the command line argument, e.g. --custom \"ecut[0]=15;nstep[0]=100\""
    )
        
    ag.add_argument("--custom-file", type=str, default=None,
        help="Specify the file containing the custom style abinit params. The privilege of --custom-file is lower than --custom."
    )

def abinit_processor(args):
    params = {}

    print("working directory: %s" % args.directory)
    if args.calc.lower() == "static":
        from atomsciflow.abinit import Static
        job = Static()
    elif args.calc.lower() == "opt":
        from atomsciflow.abinit import Opt
        job = Opt()
    elif args.calc.lower() == "vcopt":
        from atomsciflow.abinit import VcOpt
        job = VcOpt()
    elif args.calc.lower() == "md":
        from atomsciflow.abinit import MD
        job = MD()
    elif args.calc.lower() == "phonopy":
        from atomsciflow.abinit import Phonopy
        job = Phonopy()
        job.job.set_run("phonopy_dim_x", args.phonopy_dim[0])
        job.job.set_run("phonopy_dim_y", args.phonopy_dim[1])
        job.job.set_run("phonopy_dim_z", args.phonopy_dim[2])
    elif args.calc.lower() == "dfpt-epd":
        from atomsciflow.abinit import DfptElasticPiezoDielec
        job = DfptElasticPiezoDielec()
    elif args.calc.lower() == "band":
        from atomsciflow.abinit import Band
        job = Band()
        from atomsciflow.cpp.base import Kpath
        kpath = Kpath()
        if args.kpath.count(";") != 0:
            kpath.read(args.kpath)
        else:
            kpath.read_file(args.kpath)
        job.set_kpath(kpath)
    elif args.calc.lower() == "dos":
        from atomsciflow.abinit import Dos
        job = Dos()
    else:
        print("The specified calculation type is unfound!")
        sys.exit(1)

    job.set_pot(args.pot)
    set_calc_processor_common(job, args)
    if args.custom_file != None:
        from atomsciflow.abinit.io import read_params
        read_params(job, args.custom_file)
    if args.custom != None:
        custom_str = args.custom.replace(" ", "")
        for item in custom_str.split(";"):
            if item == "":
                continue
            if item.split("=")[1].count(",") > 0:
                params[item.split("=")[0]] = [value for value in item.split("=")[1].split(",")]
            else:
                params[item.split("=")[0]] = item.split("=")[1]
    for item in params:
        if params[item] == None:
            continue
        job.set_param(item, params[item])
    job.run(args.directory)
