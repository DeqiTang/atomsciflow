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

import os
import sys
import argparse

from atomsciflow.cmd.cmd_utils import log_cmd_start, log_sub_cmd_start, log_sub_cmd_end
from atomsciflow.cmd.atomsciflow_post_cp2k import add_cp2k_post_subparser, cp2k_post_processor

def main():
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest="subcommand", title="subcommand", description="choose one and only one subcommand")
    
    add_cp2k_post_subparser(subparsers)
    #
    args = parser.parse_args()

    if 1 == len(sys.argv):
        parser.print_help()
        sys.exit(1)        

    log_cmd_start("atomsciflow-post")

    if args.subcommand == "gamessus":
        pass
    if args.subcommand == "vasp":
        pass
    if args.subcommand == "cp2k":
        log_sub_cmd_start(args.subcommand)
        cp2k_post_processor(args)
        log_sub_cmd_end(args.subcommand)
    if args.subcommand == "abinit":
        pass
    if args.subcommand == "elk":
        pass
    if args.subcommand == "qe":
        pass
    if args.subcommand == "qmcpack":
        pass
    if args.subcommand == "siesta":
        pass
    if args.subcommand == "dalton":
        pass                                     
    if args.subcommand == "octopus":
        pass                                     
    if args.subcommand == "nwchem":
        pass                                       
    if args.subcommand == "orca":
        pass                                      
    if args.subcommand == "gaussian":
        pass                                      
    if args.subcommand == "qchem":
        pass                                     
    if args.subcommand == "exciting":
        pass                                      
    if args.subcommand == "xtb":
        pass                                      
    if args.subcommand == "gromacs":
        pass                                  
    if args.subcommand == "lammps":
        pass                                   
    if args.subcommand == "dftbplus":
        pass

if __name__ == "__main__":
    main()
