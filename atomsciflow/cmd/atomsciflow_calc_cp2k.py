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

def add_cp2k_subparser(subparsers):
    subparser = subparsers.add_parser("cp2k", 
        help="The CP2K calculator")

    subparser.add_argument("-d", "--directory", type=str, default="atomsciflow-calc-running-dir",
        help="The working directory where calculation is happening")

    ag = subparser.add_argument_group(title="Structure", description="Specification of structure files")

    ag.add_argument("--xyz", type=str, default=None, required=True,
        help="Specify the xyz structure file")

    subparser.add_argument("-c", "--calc", type=str, default="static",
        choices=["static", "opt", "vcopt", "vib", "md", "metamd"],
        help="The calculation to do. The specified value is case insensitive")

    subparser.add_argument("--runopt", type=str, default="gen",
        choices=["gen", "run", "genrun"],
        help="The running option. gen -> only generate the files; run -> run the calculation generated by previous gen; genrun -> generation and run the calculation")

    subparser.add_argument("-a", "--auto-level", type=int, default=0,
        choices=[0, 1, 2, 3],
        help="The automation level of the task")

    subparser.add_argument("--server", type=str, default="pbs",
        choices=["pbs", "llhpc", "yhbatch", "lsf_sz", "lsf_sustc", "cdcloud"])

def cp2k_processor(args):
    print("working directory: %s" % args.directory)
    if args.calc.lower() == "static":
        from atomsciflow.cp2k import Static
        job = Static()
        job.get_xyz(args.xyz)
        job.job.set_run("runopt", args.runopt)
        job.job.set_run("server", args.server)
        job.set_job_steps_default()
        job.run(args.directory)
    elif args.calc.lower() == "opt":
        from atomsciflow.cp2k import Opt
        job = Opt()
        job.get_xyz(args.xyz)
        job.job.set_run("runopt", args.runopt)
        job.job.set_run("server", args.server)
        job.set_job_steps_default()
        job.run(args.directory)
    elif args.calc.lower() == "vcopt":
        from atomsciflow.cp2k import VcOpt
        job = VcOpt()
        job.get_xyz(args.xyz)
        job.job.set_run("runopt", args.runopt)
        job.job.set_run("server", args.server)        
        job.set_job_steps.default()
        job.run(args.directory)
    elif args.calc.lower() == "vib":
        from atomsciflow.cp2k import Vib
        job = Vib()
        job.get_xyz(args.xyz)
        job.job.set_run("runopt", args.runopt)
        job.job.set_run("server", args.server)
        job.set_job_steps.default()
        job.run(args.directory)        
    elif args.calc.lower() == "md":
        from atomsciflow.cp2k import MD
        job = MD()
        job.get_xyz(args.xyz)
        job.job.set_run("runopt", args.runopt)
        job.job.set_run("server", args.server)
        job.set_job_steps.default()
        job.run(args.directory)
    elif args.calc.lower() == "metamd":
        from atomsciflow.cp2k import MetaMD
        job = MetaMD()
        job.get_xyz(args.xyz)
        job.job.set_run("runopt", args.runopt)
        job.job.set_run("server", args.server)
        job.set_job_steps_default()
        job.run(args.directory)        
    else:
        print("The specified calculation type is unfound!")
        sys.exit(1)
    