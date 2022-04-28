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

def add_calc_parser_common_structure(subparser):

    ag = subparser.add_mutually_exclusive_group(required=True)

    ag.add_argument("--xyz", type=str, default=None,
        help="Specify the xyz structure file")
    
    ag.add_argument("--cif", type=str, default=None,
        help="Spedify the cif structure file")
    
def add_calc_parser_common(subparser):
    
    add_calc_parser_common_structure(subparser)

    subparser.add_argument("-d", "--directory", type=str, default="atomsciflow-calc-running-dir",
        help="The working directory where calculation is happening")

    subparser.add_argument("-a", "--auto-level", type=int, default=0,
        choices=[0, 1, 2, 3, 4, 5, 6],
        help="The automation level of the task. 0 -> doing nothing; 1 -> generate files only; 2 -> generate and run locally using bash directly; 3 -> generate and submit job to job scheduler locally; 4 -> generate and copy to remote server; 5 -> generate and copy to remote server, then run remotely using bash directly; 6 -> generate and copy to remote server, then submit it to remote server job scheduler;")

    subparser.add_argument("--server", type=str, default="pbs",
        choices=["pbs", "llhpc", "yhbatch", "lsf_sz", "lsf_sustc", "cdcloud"])

def set_calc_processor_common(calc, args):
    calc.job.set_run("auto_level", args.auto_level)
    calc.job.set_run("server", args.server)  
