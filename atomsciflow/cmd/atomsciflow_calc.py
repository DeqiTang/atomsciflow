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
from atomsciflow.cmd.atomsciflow_calc_gamessus import add_gamessus_subparser, gamessus_processor
from atomsciflow.cmd.atomsciflow_calc_vasp import add_vasp_subparser, vasp_processor
from atomsciflow.cmd.atomsciflow_calc_cp2k import add_cp2k_subparser, cp2k_processor
from atomsciflow.cmd.atomsciflow_calc_abinit import add_abinit_subparser, abinit_processor
from atomsciflow.cmd.atomsciflow_calc_elk import add_elk_subparser, elk_processor
from atomsciflow.cmd.atomsciflow_calc_qe import add_qe_subparser, qe_processor
from atomsciflow.cmd.atomsciflow_calc_qmcpack import add_qmcpack_subparser, qmcpack_processor
from atomsciflow.cmd.atomsciflow_calc_siesta import add_siesta_subparser, siesta_processor
from atomsciflow.cmd.atomsciflow_calc_dalton import add_dalton_subparser, dalton_processor

def main():
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest="subcommand", title="subcommand", description="choose one and only one subcommand")
    
    add_gamessus_subparser(subparsers)
    add_vasp_subparser(subparsers)
    add_cp2k_subparser(subparsers)
    add_abinit_subparser(subparsers)
    add_elk_subparser(subparsers)
    add_qe_subparser(subparsers)
    add_qmcpack_subparser(subparsers)
    add_siesta_subparser(subparsers)
    add_dalton_subparser(subparsers)
    #
    args = parser.parse_args()

    if 1 == len(sys.argv):
        parser.print_help()
        sys.exit(1)        

    log_cmd_start("atomsciflow")

    if args.subcommand == "gamessus":
        log_sub_cmd_start(args.subcommand)
        gamessus_processor(args)
        log_sub_cmd_end(args.subcommand)
    if args.subcommand == "vasp":
        log_sub_cmd_start(args.subcommand)
        vasp_processor(args)
        log_sub_cmd_end(args.subcommand)
    if args.subcommand == "cp2k":
        log_sub_cmd_start(args.subcommand)
        cp2k_processor(args)
        log_sub_cmd_end(args.subcommand)
    if args.subcommand == "abinit":
        log_sub_cmd_start(args.subcommand)
        abinit_processor(args)
        log_sub_cmd_end(args.subcommand)
    if args.subcommand == "elk":
        log_sub_cmd_start(args.subcommand)
        elk_processor(args)
        log_sub_cmd_end(args.subcommand)
    if args.subcommand == "qe":
        log_sub_cmd_start(args.subcommand)
        qe_processor(args)
        log_sub_cmd_end(args.subcommand)
    if args.subcommand == "qmcpack":
        log_sub_cmd_start(args.subcommand)
        qmcpack_processor(args)
        log_sub_cmd_end(args.subcommand)   
    if args.subcommand == "siesta":
        log_sub_cmd_start(args.subcommand)
        siesta_processor(args)
        log_sub_cmd_end(args.subcommand)   
    if args.subcommand == "dalton":
        log_sub_cmd_start(args.subcommand)
        dalton_processor(args)
        log_sub_cmd_end(args.subcommand)                                        

if __name__ == "__main__":
    main()
