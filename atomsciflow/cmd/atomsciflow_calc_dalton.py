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

def add_dalton_subparser(subparsers):
    subparser = subparsers.add_parser("dalton", 
        help="The Dalton calculator")

    subparser.add_argument("-c", "--calc", type=str, default="static",
        choices=["static", "opt", "md"],
        help="The calculation to do. The specified value is case insensitive")

    add_calc_parser_common(subparser)

def dalton_processor(args):
    print("working directory: %s" % args.directory)
    if args.calc.lower() == "static":
        from atomsciflow.dalton import Dalton
        job = Dalton()
        job.get_xyz(args.xyz)
        set_calc_processor_common(job, args)      
        job.run(args.directory)
    elif args.calc.lower() == "opt":
        from atomsciflow.dalton import Dalton
        job = Dalton()
        job.get_xyz(args.xyz)
        set_calc_processor_common(job, args)      
        job.run(args.directory)
    else:
        print("The specified calculation type is unfound!")
        sys.exit(1)
    