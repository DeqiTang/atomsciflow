/************************************************************************
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
************************************************************************/

/// @file src/atomsciflow/server/submit_script.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Mon 07 Mar 2022 10:14:15 PM CST

#include "atomsciflow/server/submit_script.h"

#include <string>
#include <sstream>
#include <map>
#include <boost/format.hpp>

namespace atomsciflow {

std::string submit_header_llhpc(std::map<std::string, std::string>& params) {
    std::string out = "";
    out += "#!/bin/bash\n";
    out += (boost::format("#SBATCH -p %1%\n") % params["partition"]).str();
    out += (boost::format("#SBATCH -N %1%\n") % params["nodes"]).str();
    out += (boost::format("#SBATCH -n %1%\n") % params["ntasks_per_node"]).str();
    out += (boost::format("#SBATCH -J %1%\n") % params["jobname"]).str();
    out += (boost::format("#SBATCH -o %1%\n") % params["stdout"]).str();
    out += (boost::format("#SBATCH -e %1%\n") % params["stderr"]).str();
    out += (boost::format("ABSOLUTE_WORK_DIR=%1%\n") % params["absolute_work_dir"]).str();
    out += "CMD_HEAD=yhrun\n";

    return out;
}

std::string submit_header_yh(std::map<std::string, std::string>& params) {
    std::string out;
    out += "#!/bin/bash\n";

    out += "ABSOLUTE_WORK_DIR=";
    out += params["absolute_work_dir"];
    out += "\n\n";
    out += "CMD_HEAD=yhrun\n";

    return out;
}

std::string submit_header_pbs(std::map<std::string, std::string>& params) {
    std::ostringstream out;

    out << "#!/bin/bash\n";
    out << "#PBS -N " << params["jobname"] << "\n";
    out << "#PBS -l nodes="
        << params["nodes"]
        << ":ppn="
        << params["ppn"]
        << "\n";
    if ("" != params["queue"]) {
        out << boost::format("#PBS -q %1%\n") % params["queue"];
    }
    out << "\n";
    out << "cd $PBS_O_WORKDIR\n";

    out << "ABSOLUTE_WORK_DIR=" << params["absolute_work_dir"];
    out << "\n\n";

    out << "CMD_HEAD=\"mpirun -np $NP -machine $PBS_NODEFILE\"\n";

    return out.str();
}

std::string submit_header_bash(std::map<std::string, std::string>& params) {
    std::string out;
    out += "#!/bin/bash\n";
    out += "\n";
    out += (boost::format("ABSOLUTE_WORK_DIR=%1%\n") % params["absolute_work_dir"]).str();
    out += "\n";
    out += (boost::format("CMD_HEAD=%1%\n") % params["mpi"]).str();
    out += "\n";

    return out;
}

std::string submit_header_lsf_sz(std::map<std::string, std::string>& params) {
    std::string out;
    out += "#!/bin/bash\n";
    out += (boost::format("APP_NAME=%1%\n") % params["queue"]).str();
    out += (boost::format("NP=%1%\n") % params["np"]).str();
    out += (boost::format("NP_PER_NODE=%1%\n") % params["np_per_node"]).str();
    out += "RUN=\"RAW\"\n";
    out += "CURDIR=$PWD\n";
    out += "source /home-yg/env/intel-12.1.sh\n";
    out += "source /home-yg/env/openmpi-1.6.5-intel.sh\n";
    out += "cd $CURDIR\n";
    out += "# starting creating ./nodelist\n";
    out += "rm -rf $CURDIR/nodelist >& /dev/null\n";
    out += "for i in `echo $LSB_HOSTS`\n";
    out += "do\n";
    out += "  echo \"$i\" >> $CURDIR/nodelist \n";
    out += "done\n";
    out += "ndoelist=$(cat $CURDIR/nodelist | uniq | awk \'{print $1}\' | tr \'\\n\' \',\')\n";

    out += (boost::format("ABSOLUTE_WORK_DIR=%1%\n") % params["absolute_work_dir"]).str();
    out += "\n";
    out += "CMD_HEAD=\"mpirun -np $NP -machinefile $CURDIR/nodelist\"\n";

    return out;
}

std::string submit_header_lsf_sustc(std::map<std::string, std::string>& params) {
    std::string out;
    out += "#!/bin/bash\n";
    out += (boost::format("$BSUB -J %1%\n") % params["jobname"]).str();
    out += "#BSUB -e %J.err\n";
    out += "#BSUB -o %J.out\n";
    out += (boost::format("#BSUB -q %1%\n") % params["queue"]).str();
    out += (boost::format("#BSUB -n %1%\n") % params["np"]).str(); // number of total cores
    out += (boost::format("#BSUB -R\"span[ptile=%1%]\"\n") % params["np_per_node"]).str();
    out += "hostfile=`echo $LSB_DJOB_HOSTFILE`\n";
    out += "NP=`cat $hostfile | wc -l`\n";
    out += "cd $LS_SUBCWD\n";

    out += (boost::format("ABSOLUTE_WORK_DIR=%1%\n") % params["absolute_work_dir"]).str();
    out += "\n";
    out += "CMD_HEAD=\"mpirun -machinefile $LSB_DJOB_HOSTFILE -np $NP\"\n";

    return out;
}

std::string submit_header_cdcloud(std::map<std::string, std::string>& params) {
    std::string out;

    out += "#!/bin/bash\n";
    out += (boost::format("#SBATCH --partition=%1%\n") % params["partition"]).str();
    out += (boost::format("#SBATCH --nodes=%1%\n") % params["nodes"]).str();
    out += (boost::format("#SBATCH --ntasks-per-node=%1%\n") % params["ntasks_per_node"]).str();
    out += (boost::format("#SBATCH --job-name=%1%\n") % params["jobname"]).str();
    out += (boost::format("#SBATCH --output=%1%\n") % params["stdout"]).str();
    out += (boost::format("#SBATCH --error=%1%\n") % params["stderr"]).str();
    out += "#\n";
    out += "export I_MPI_PMI_LIBRARY=/opt/gridview/slurm/lib/libpmi.so\n";
    // out += "export FORT_BUFFERED=1\n"; // may arouse error for running of quantum espresso 

    out += (boost::format("ABSOLUTE_WORK_DIR=%1%\n") % params["absolute_work_dir"]).str();
    out += "\n";
    out += "CMD_HEAD=\"srun --mpi=pmix_v3\"\n";

    return out;
}

std::string submit_header_slurm(std::map<std::string, std::string>& params) {
    std::string out;

    out += "#!/bin/bash\n";
    out += (boost::format("#SBATCH --partition=%1%\n") % params["partition"]).str();
    out += (boost::format("#SBATCH --nodes=%1%\n") % params["nodes"]).str();
    out += (boost::format("#SBATCH --ntasks-per-node=%1%\n") % params["ntasks_per_node"]).str();
    out += (boost::format("#SBATCH --time=%1%\n") % params["time"]).str();
    out += (boost::format("#SBATCH --account=%1%\n") % params["account"]).str();
    out += (boost::format("#SBATCH --constraint=%1%\n") % params["constraint"]).str();
    out += (boost::format("#SBATCH --output=%1%\n") % params["stdout"]).str();
    out += (boost::format("#SBATCH --error=%1%\n") % params["stderr"]).str();
    out += (boost::format("#SBATCH --job-name=%1%\n") % params["jobname"]).str();
    out += "#\n";

    out += (boost::format("ABSOLUTE_WORK_DIR=%1%\n") % params["absolute_work_dir"]).str();
    out += "\n";
    out += "CMD_HEAD=\"srun\"\n";

    return out;
}

} //namespace atomsciflow
