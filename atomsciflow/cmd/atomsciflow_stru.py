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


def read_structure(crystal, filepath):
    if filepath.endswith(".cif"):
        from atomsciflow.cpp.parser import read_cif_file
        read_cif_file(crystal, filepath)
    elif filepath.endswith(".xyz"):
        from atomsciflow.cpp.parser import read_xyz_file
        read_xyz_file(crystal, filepath)

def write_structure(crystal, filepath):
    if filepath.endswith(".cif"):
        from atomsciflow.cpp.parser import write_cif_file
        write_cif_file(crystal, filepath)
    elif filepath.endswith(".xyz"):
        from atomsciflow.cpp.parser import write_xyz_file
        write_xyz_file(crystal, filepath)

def main():
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers(dest="subcommand", title="subcommand", description="choose one and only one subcommand")
    
    #
    subparser = subparsers.add_parser("convert", 
        help="Convert between different structure file formats")

    subparser.add_argument("-i", "--input", type=str, required=True,
        help="Specify the input structure")

    subparser.add_argument("-o", "--output", type=str, required=True,
        help="Specify the outout structure")

    #
    subparser = subparsers.add_parser("supercell", 
        help="Build the supercell")

    subparser.add_argument("-i", "--input", type=str, required=True,
        help="Specify the input structure")

    subparser.add_argument("-o", "--output", type=str, required=True,
        help="Specify the outout structure")

    subparser.add_argument("-n", "--n", type=int, nargs="+",
        help="Specify number of replicas for each lattice direction, respectively")

    #
    subparser = subparsers.add_parser("redefine", 
        help="Redefine the lattice vectors of the input structure.")

    subparser.add_argument("-i", "--input", type=str, required=True,
        help="Specify the input structure")

    subparser.add_argument("-o", "--output", type=str, required=True,
        help="Specify the outout structure")

    subparser.add_argument("-a", "--a", type=int, nargs="+",
        help="Specify three integers to define the new a vector")

    subparser.add_argument("-b", "--b", type=int, nargs="+",
        help="Specify three integers to define the new b vector")

    subparser.add_argument("-c", "--c", type=int, nargs="+",
        help="Specify three integers to define the new c vector")

    subparser.add_argument("--precision", type=float, default=1.0e-8,
        help="One positive float as close to 0 as possible.")

    subparser.add_argument("-r", "--rotation", type=str, nargs="+",
        help="Specifying the rotation matrix")

    #
    subparser = subparsers.add_parser("cleave", 
        help="Cleave the surface")

    subparser.add_argument("-i", "--input", type=str, required=True,
        help="Specify the input structure")

    subparser.add_argument("-o", "--output", type=str, required=True,
        help="Specify the outout structure")

    subparser.add_argument("--direction", type=int, nargs="+",
        help="Specify the direction of the surface plane to cleave.")

    subparser.add_argument("--thick", type=float, default=15,
        help="Specify the thickness of the vacuum layer.")

    subparser.add_argument("--precision", type=float, default=1.0e-8,
        help="One positive float as close to 0 as possible.")

    #
    subparser = subparsers.add_parser("vacuum", 
        help="Set vacuum layer on the specified plane.")

    subparser.add_argument("-i", "--input", type=str, required=True,
        help="Specify the input structure")

    subparser.add_argument("-o", "--output", type=str, required=True,
        help="Specify the outout structure")

    subparser.add_argument("--plane", type=str, required=True,
        choices=["ab", "ac", "bc"],
        help="Choose the plane to add vacuum layer.")

    subparser.add_argument("--thick", type=float, required=True,
        help="The thickness of the vacuum layer to set")

    #
    subparser = subparsers.add_parser("tube", 
        help="Build the nanotube")

    subparser.add_argument("-i", "--input", type=str, required=True,
        help="Specify the input structure")

    subparser.add_argument("-o", "--output", type=str, required=True,
        help="Specify the outout structure")

    subparser.add_argument("--axis", type=str, required=True,
        choices=["a", "b", "c"],
        help="Build nanotube along an axis parallel to axis specified, can be: a, b or c")

    subparser.add_argument("--plane", type=str, required=True,
        choices=["ab", "ac", "bc"],
        help="On which plane to add vacuum layer")

    #
    subparser = subparsers.add_parser("merge", 
        help="Merge to structure, usually slab")

    subparser.add_argument("-i", "--input", type=str, nargs=2, required=True,
        help="Specify the input structures")

    subparser.add_argument("-o", "--output", type=str, required=True,
        help="Specify the outout structure")

    subparser.add_argument("--usecell", type=int, default=0,
        help="use cell of structure 1 or 2, otherwise 0 (avearge) by default")

    subparser.add_argument("--thick", type=float, default=15,
        help="thickness of the vacuum layer")

    subparser.add_argument("--distance", type=float, default=3.4,
        help="distance between the layer, in unit of Angstromg, default is 3.4")

    #
    subparser = subparsers.add_parser("move", 
        help="Move the specified atoms along one direction by specified displacement.")

    subparser.add_argument("-i", "--input", type=str, required=True,
        help="Specify the input structure")

    subparser.add_argument("-o", "--output", type=str, required=True,
        help="Specify the outout structure")

    subparser.add_argument("--atoms", type=str, nargs="+",
        default=["all"],
        help="atoms to move, either specify a list of number with index start from 1, or specify all, which means move all the atoms toward some direction")

    subparser.add_argument("--direction", type=int, nargs=3,
        help="Specify the direction to move the atoms, in format of crystal orientation index")

    subparser.add_argument("--disp", type=float,
        help="Specify displacement along the moving direction, in unit of Anstrom")

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

    if args.subcommand == "convert":
        log_sub_cmd_start(args.subcommand)
        from atomsciflow.cpp.base import Crystal
        crystal = Crystal()
        read_structure(crystal, args.input)
        write_structure(crystal, args.output)
        log_sub_cmd_end(args.subcommand)
    elif args.subcommand == "supercell":
        log_sub_cmd_start(args.subcommand)
        from atomsciflow.cpp.base import Crystal
        crystal = Crystal()
        read_structure(crystal, args.input)

        crystal.build_supercell(args.n)

        write_structure(crystal, args.output)
        log_sub_cmd_end(args.subcommand)
    elif args.subcommand == "redefine":
        log_sub_cmd_start(args.subcommand)
        from atomsciflow.cpp.base import Crystal
        from atomsciflow.cpp.utils import redefine_lattice
        crystal = Crystal()
        read_structure(crystal, args.input)

        redefine_lattice(crystal, args.a, args.b, args.c, args.precision)

        write_structure(crystal, args.output)
        log_sub_cmd_end(args.subcommand)  
    elif args.subcommand == "cleave":
        log_sub_cmd_start(args.subcommand)
        from atomsciflow.cpp.base import Crystal
        from atomsciflow.cpp.utils import cleave_surface
        crystal = Crystal()
        read_structure(crystal, args.input)

        cleave_surface(crystal, args.direction, args.thick, args.precision)

        write_structure(crystal, args.output)
        log_sub_cmd_end(args.subcommand)  
    elif args.subcommand == "vacuum":
        log_sub_cmd_start(args.subcommand)
        from atomsciflow.cpp.base import Crystal
        from atomsciflow.cpp.utils import vacuum_layer
        crystal = Crystal()
        read_structure(crystal, args.input)

        vacuum_layer(crystal, args.plane, args.thick)

        write_structure(crystal, args.output)
        log_sub_cmd_end(args.subcommand)  

    elif args.subcommand == "merge":
        log_sub_cmd_start(args.subcommand)
        from atomsciflow.cpp.base import Crystal
        from atomsciflow.cpp.utils import merge_layers

        crystal1 = Crystal()
        crystal2 = Crystal()
        read_structure(crystal1, args.input[0])
        read_structure(crystal2, args.input[1])

        crystal = merge_layers(crystal1, crystal2, args.usecell, args.distance, args.thick)
        
        write_structure(crystal, args.output)
        log_sub_cmd_end(args.subcommand)  
    elif args.subcommand == "move":
        log_sub_cmd_start(args.subcommand)
        from atomsciflow.cpp.base import Crystal
        from atomsciflow.cpp.utils import move_along

        crystal = Crystal()
        read_structure(crystal, args.input)

        atoms_to_move = []
        if "all" in args.atoms:
            atoms_to_move = [i for i in range(crystal.natom())]
        else:
            atoms_to_move = [int(i) -1 for i in args.atoms]
        move_along(crystal, atoms_to_move, args.direction, args.disp)

        write_structure(crystal, args.output)
        log_sub_cmd_end(args.subcommand)
    elif args.subcommand == "tube":
        log_sub_cmd_start(args.subcommand)
        from atomsciflow.cpp.base import Crystal
        crystal = Crystal()
        read_structure(crystal, args.input)

        if "ab" == args.plane:
            from atomsciflow.cpp.utils import build_nanotube_ab
            crystal = build_nanotube_ab(crystal, args.axis)
        elif "ac" == args.plane:
            from atomsciflow.cpp.utils import build_nanotube_bc
            crystal = build_nanotube_ac(crystal, args.axis)
        elif "bc" == args.plane:
            from atomsciflow.cpp.utils import build_nanotube_ac
            crystal = build_nanotube_bc(crystal, args.axis)

        write_structure(crystal, args.output)
        log_sub_cmd_end(args.subcommand) 
    elif args.subcommand == "tidy":
        log_sub_cmd_start(args.subcommand)
        print("not implemented yet!")
        log_sub_cmd_end(args.subcommand)


if __name__ == "__main__":
    main()
