"""
MIT License

Copyright (c) 2022 Deqi Tang

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

import os
import sys
import argparse

from atomsciflow.cmd.cmd_utils import log_cmd_start, log_sub_cmd_start, log_sub_cmd_end


def main():
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest="subcommand", title="subcommand", description="choose one and only one subcommand")
    
    #
    subparser = subparsers.add_parser("redefine", 
        help="Redefine the lattice vectors")

    subparser.add_argument("-i", "--input", type=str, required=True,
        help="Specify the input structure")

    subparser.add_argument("-o", "--output", type=str, required=True,
        help="Specify the outout structure")

    subparser.add_argument("-r", "--rotation", type=str, nargs="+", required=True,
        help="Specifying the rotation matrix")
    #
    subparser = subparsers.add_parser("tidy", 
        help="Tidy the structure")

    subparser.add_argument("-i", "--input", type=str, required=True,
        help="Specify the input structure")

    subparser.add_argument("-o", "--output", type=str, required=True,
        help="Specify the outout structure")
    #
    args = parser.parse_args()

    if 1 == len(sys.argv):
        parser.print_help()
        sys.exit(1)        

    log_cmd_start("atomsciflow-post")

    if args.subcommand == "redefine":
        log_sub_cmd_start(args.subcommand)
        print("not implemented yet!")
        log_sub_cmd_end(args.subcommand)  
    if args.subcommand == "tidy":
        log_sub_cmd_start(args.subcommand)
        print("not implemented yet!")
        log_sub_cmd_end(args.subcommand)


if __name__ == "__main__":
    main()
