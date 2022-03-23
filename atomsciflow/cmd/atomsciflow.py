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

def main():
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest="driver", title="subcommands", description="choose one and only one subcommands")
    # --------------------------------------------------------------------------
    #  Test
    # --------------------------------------------------------------------------
    subparser = subparsers.add_parser("version", help="running version")

    #subparser.add_argument("")

    # ==========================================================
    # transfer parameters from the praser
    # ==========================================================
    args = parser.parse_args()

    if len(sys.argv) == 1:
        # display help message when no args provided
        parser.print_help()
        sys.exit(1)        

    log_cmd_start("atomsciflow")

    log_sub_cmd_start(args.driver)

    if args.driver == "version":
        from atomsciflow.cpp.utils import version
        print("the version is:", version())

    log_sub_cmd_end(args.driver)

if __name__ == "__main__":
    main()


