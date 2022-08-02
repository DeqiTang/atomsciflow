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

def add_nwchem_subparser(subparsers):
    subparser = subparsers.add_parser("nwchem", 
        help="The NWChem calculator")

    add_calc_parser_common(subparser)

    subparser.add_argument("-c", "--calc", type=str, default="static",
        choices=["static", "opt", "md", "freq", "neb"],
        help="The calculation to do. The specified value is case insensitive")

    ag = subparser.add_argument_group(title="custom")

    ag.add_argument("--custom", type=str, default=None,
        help="Specify parameters that are not provided directly in the command line argument, e.g. --custom \"print=low;start=nwchem-job\""
    )
    
    ag.add_argument("--custom-file", type=str, default=None,
        help="Specify the file containing the custom style nwchem params. The privilege of --custom-file is lower than --custom."
    )

    ag = subparser.add_argument_group(title="neb")

    ag.add_argument("--neb-images", type=str, nargs="+",
        help="Specify the structure images to do neb calculation."
    )

    ag.add_argument("--neb-images-mode", type=int, default=0,
        choices=[0, 1, 2],
        help="Specify the mode of input for structure images to do neb calculation"
    )

def nwchem_processor(args):

    params = {}

    params["add_keyword"] = {}
    params["set_keywords"] = {}
    if args.custom != None:
        custom_str = args.custom.replace(" ", "") # remove all space
        for item in custom_str.split(";"):
            if item == "":
                continue
            if item.count("+=") > 0:
                if item.split("+=")[1].count(",") > 0:
                    params["add_keyword"][item.split("+=")[0]] = [value for value in item.split("+=")[1].split(",")]
                else:
                    params["add_keyword"][item.split("+=")[0]] = [item.split("+=")[1]]
            else:
                    params["set_keywords"][item.split("=")[0]] = [value for value in item.split("=")[1].split(",")]

    print("working directory: %s" % args.directory)
    if args.calc.lower() == "static":
        from atomsciflow.nwchem import Static
        job = Static()
    elif args.calc.lower() == "opt":
        from atomsciflow.nwchem import Opt
        job = Opt()
    elif args.calc.lower() == "md":
        from atomsciflow.nwchem import MD
        job = MD()
    elif args.calc.lower() == "freq":
        from atomsciflow.nwchem import Freq
        job = Freq()
    elif args.calc.lower() == "neb":
        from atomsciflow.nwchem import Neb
        job = Neb()
    else:
        print("The specified calculation type is unfound!")
        sys.exit(1)

    if args.calc.lower() == "neb":
        import os
        job.set_neb_images(
            [os.path.abspath(item) for item in args.neb_images], 
            mode=args.neb_images_mode
        )
    set_calc_processor_common(job, args)
    if args.custom_file != None:
        from atomsciflow.nwchem.io import read_params
        read_params(job, args.custom_file)
    for item in params["add_keyword"]:
        if params["add_keyword"][item] == None:
            continue
        for keyword in params["add_keyword"][item]:
            job.add_keyword(item, keyword)
    for item in params["set_keywords"]:
        if params["set_keywords"][item] == None:
            continue
        job.set_keywords(item, params["set_keywords"][item])
    job.run(args.directory)
