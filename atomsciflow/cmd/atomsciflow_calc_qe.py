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

def add_qe_subparser(subparsers):
    subparser = subparsers.add_parser("qe", 
        help="The Quantum Espresso calculator")

    subparser.add_argument("-c", "--calc", type=str, default="static",
        choices=["static", "opt", "vcopt", "bomd", "phonopy", "band", "dos"],
        help="The calculation to do. The specified value is case insensitive")

    add_calc_parser_common(subparser)

    # custom
    ag = subparser.add_argument_group(title="custom")
    
    ag.add_argument("--custom", type=str, default=None,
        help="Specify parameters that are not provided directly in the command line argument, e.g. --custom \"control/nstep=1;electrons/conv_thr=1.0e-7\""
    )

    ag.add_argument("--custom-file", type=str, default=None,
        help="Specify the file containing the custom style qe params. The privilege of --custom-file is lower than --custom."
    )

def qe_processor(args):
    
    print("working directory: %s" % args.directory)
    if args.calc.lower() == "static":
        from atomsciflow.qe import Static
        job = Static()
    elif args.calc.lower() == "opt":
        from atomsciflow.qe import Opt
        job = Opt()
    elif args.calc.lower() == "vcopt":
        from atomsciflow.qe import VcOpt
        job = VcOpt()
    elif args.calc.lower() == "phonopy":
        from atomsciflow.qe import Phonopy
        job = Phonopy()
        job.job.set_run("phonopy_dim_x", args.phonopy_dim[0])
        job.job.set_run("phonopy_dim_y", args.phonopy_dim[1])
        job.job.set_run("phonopy_dim_z", args.phonopy_dim[2])
    elif args.calc.lower() == "band":
        from atomsciflow.qe import Band
        job = Band()
        from atomsciflow.cpp.base import Kpath
        kpath = Kpath()
        if args.kpath.count(";") != 0:
            kpath.read(args.kpath)
        else:
            kpath.read_file(args.kpath)
        job.set_kpath(kpath)
    elif args.calc.lower() == "dos":
        from atomsciflow.qe import Dos
        job = Dos()
    elif args.calc.lower() == "bomd":
        from atomsciflow.qe import BOMD
        job = BOMD()
    else:
        print("The specified calculation type is unfound!")
        import sys
        sys.exit(1)
    
    set_calc_processor_common(job, args)
    if args.custom_file != None:
        from atomsciflow.qe.io import read_params
        read_params(job, args.custom_file)  
    if args.custom != None:
        custom_str = args.custom.replace(" ", "")
        for item in custom_str.split(";"):
            if item == "":
                continue
            if item.count(":=") > 0:
                if item.split(":=")[1].count("|") > 0:
                    i = 0
                    for row in item.split(":=")[1].split("|"):
                        job.set_card_data(
                            item.split(":=")[0],
                            row.split(","),
                            i
                        )
                        i = i +1
                else:
                    job.set_card_data(
                        item.split(":=")[0],
                        item.split(":=")[1].split(","),
                        0
                    )
            else:
                if item.split("=")[1].count(",") > 0:
                    job.set_param(
                        item.split("=")[0].split("/")[0],
                        item.split("=")[0].split("/")[1],
                        [value for value in item.split("=")[1].split(",")]
                    )
                else:
                    job.set_param(
                        item.split("=")[0].split("/")[0],
                        item.split("=")[0].split("/")[1],
                        item.split("=")[1]
                    )
    job.run(args.directory)
