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

def add_octopus_subparser(subparsers):
    subparser = subparsers.add_parser("octopus", 
        help="The Octopus calculator")

    add_calc_parser_common(subparser)
    
    subparser.add_argument("-c", "--calc", type=str, default="static",
        choices=["static", "opt", "md"],
        help="The calculation to do. The specified value is case insensitive")

    ag = subparser.add_argument_group(title="Octopus setting", 
        description="Specify Octopus parameters")
    
    ag.add_argument("--spacing", type=float, default=0.2,
        help="The spacing between the points in the mesh.")

    subparser.add_argument("--custom", type=str, default=None,
        help="Specify parameters that are not provided directly in the command line argument, e.g. --custom \"ExtraStates=1;Spacing=0.2;Radius=2.5\""
    )

def octopus_processor(args):

    params = {}

    params["Spacing"] = args.spacing

    if args.custom != None:
        custom_str = args.custom.replace(" ", "") # remove all space
        for item in custom_str.split(";"):
            params[item.split("=")[0]] = item.split("=")[1]

    print("working directory: %s" % args.directory)
    if args.calc.lower() == "static":
        from atomsciflow.octopus import Static
        job = Static()
    elif args.calc.lower() == "opt":
        from atomsciflow.octopus import Opt
        job = Opt()
    else:
        print("The specified calculation type is unfound!")
        sys.exit(1)
    
    job.get_xyz(args.xyz)
    set_calc_processor_common(job, args)  
    for item in params:
        if params[item] == None:
            continue
        job.set_param(item, params[item])
    job.set_job_steps_default()
    job.run(args.directory)
