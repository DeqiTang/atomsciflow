# Copyright (c) 2021 DeqiTang
# Distributed under the terms of the MIT license
# @author: DeqiTang
# email: deqi_tang@163.com 


import os
import sys
import argparse


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
    

    if args.driver == "version":
        from atomsciflow.cpp.utils import version
        print("the version is:", version())


if __name__ == "__main__":
    main()
