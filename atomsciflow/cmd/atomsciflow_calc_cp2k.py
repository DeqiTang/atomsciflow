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

def add_cp2k_subparser(subparsers):
    subparser = subparsers.add_parser("cp2k", 
        help="The CP2K calculator")

    add_calc_parser_common(subparser)

    subparser.add_argument("-c", "--calc", type=str, default="static",
        choices=["scf", "static", "band", "dos", "opt", "vcopt", "vib", "md", "metamd", "phonopy", "neb", "metamdplumed"],
        help="The calculation to do. The specified value is case insensitive")

    # neb calculation
    ag = subparser.add_argument_group(title="neb")
    
    ag.add_argument("--images", type=str, nargs="+", 
        help="Specify the images to do the neb calculation, e.g. --images initial.xyz intermidiate.xyz ... final.xyz"
    )

    # custom specification of cp2k params
    ag = subparser.add_argument_group(title="custom")
    
    ag.add_argument("--custom", type=str, default=None,
        help="Specify parameters that are not provided directly in the command line argument, e.g. --custom \"force_eval/dft/scf/max_diis=4;force_eval/dft/scf/eps_scf=1.0e-5\". The privilege of --custom is higher than --custom-file."
    )

    ag.add_argument("--custom-file", type=str, default=None,
        help="Specify the file containing the custom style cp2k params. The privilege of --custom-file is lower than --custom."
    )

    # plumed parameters
    ag = subparser.add_argument_group(title="plumed")
    
    ag.add_argument("--plumed-custom", type=str, default=None,
        help="Specify plumed parameters."
    )

def cp2k_processor(args):
    params = {}
    params["global/print_level"] = "LOW"

    if args.custom != None:
        custom_str = args.custom.replace(" ", "")
        for item in custom_str.split(";"):
            if item.isspace() or item == "":
                continue
            if item.split("=")[1].count(",") > 0:
                params[item.split("=")[0].replace(" ", "")] = [value for value in item.split("=")[1].split(",")]
            else:
                params[item.split("=")[0].replace(" ", "")] = item.split("=")[1]

    print("working directory: %s" % args.directory)
    if args.calc.lower() == "static":
        from atomsciflow.cp2k import Static
        job = Static()
        from atomsciflow.cpp.base import Kpath
        kpath = Kpath()
        if args.kpath.count(";") != 0:
            kpath.read(args.kpath)
        else:
            kpath.read_file(args.kpath)
        job.set_kpoint_set(kpath)
    elif args.calc.lower() == "band":
        from atomsciflow.cp2k import Band
        job = Band()
        from atomsciflow.cpp.base import Kpath
        kpath = Kpath()
        if args.kpath.count(";") != 0:
            kpath.read(args.kpath)
        else:
            kpath.read_file(args.kpath)
        job.set_kpoint_set(kpath)
    elif args.calc.lower() == "dos":
        from atomsciflow.cp2k import Dos
        job = Dos()
    elif args.calc.lower() == "scf":
        from atomsciflow.cp2k import Scf
        job = Scf()
    elif args.calc.lower() == "opt":
        from atomsciflow.cp2k import Opt
        job = Opt()
    elif args.calc.lower() == "vcopt":
        from atomsciflow.cp2k import VcOpt
        job = VcOpt()
    elif args.calc.lower() == "vib":
        from atomsciflow.cp2k import Vib
        job = Vib()
    elif args.calc.lower() == "md":
        from atomsciflow.cp2k import MD
        job = MD()
    elif args.calc.lower() == "metamd":
        from atomsciflow.cp2k import MetaMD
        job = MetaMD()
    elif args.calc.lower() == "metamdplumed":
        from atomsciflow.cp2k import MetaMDPlumed
        job = MetaMDPlumed()
    elif args.calc.lower() == "phonopy":
        from atomsciflow.cp2k import Phonopy
        job = Phonopy()
        job.job.set_run("phonopy_dim_x", args.phonopy_dim[0])
        job.job.set_run("phonopy_dim_y", args.phonopy_dim[1])
        job.job.set_run("phonopy_dim_z", args.phonopy_dim[2])
    elif args.calc.lower() == "neb":
        from atomsciflow.cp2k import Neb
        job = Neb()
        job.set_images(args.images)
    else:
        print("The specified calculation type is unfound!")
        sys.exit(1)
            
    set_calc_processor_common(job, args)
    if args.custom_file != None:
        from atomsciflow.cp2k.io import read_params
        read_params(job, args.custom_file)
    for item in params:
        if params[item] == None:
            continue
        job.set_param(item, params[item])
    if args.plumed_custom == None:
        pass
    else:
        for item in args.plumed_custom.split(";"):
            if item.isspace() or item == "":
                continue
            if item.split("=")[1].count(",") > 0:
                job.plumed.set_param(
                    item.split("=")[0].replace(" ", ""),
                    [value for value in item.split("=")[1].split(",")]
                )
            else:
                job.plumed.set_param(
                    item.split("=")[0].replace(" ", ""),
                    item.split("=")[1]
                )
    job.run(args.directory)
