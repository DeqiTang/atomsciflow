/************************************************************************
    > File Name: cp2k.cpp
    > Author: deqi
    > Mail: deqi_tang@163.com 
    > Created Time: Sun 31 Jan 2021 07:28:30 PM CST
************************************************************************/

#include "atomsciflow/cp2k/cp2k.h"

#include <iostream>
#include <fstream>
//#include <filesystem>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

namespace atomsciflow {

Cp2kSectionV1& Cp2k::set_subsys(Crystal& crystal) {
    this->crystal = crystal;
    return this->set_subsys();
}

Cp2kSectionV1& Cp2k::set_subsys() {
    auto& subsys = this->sections["force_eval"].add_subsection("subsys"); 
    auto& cell = subsys.add_subsection("cell");
    cell.set_param("a", this->crystal.cell[0]);
    cell.set_param("b", this->crystal.cell[1]);
    cell.set_param("c", this->crystal.cell[2]);
    
    // std::cout << "cell set finished!\n"; 
    auto& coord = subsys.add_subsection("coord");
    std::vector<std::vector<std::string>> matrix_str;
    for (auto atom : this->crystal.atoms) {
        matrix_str.push_back(std::vector<std::string>{
                atom.name,
                std::to_string(atom.x),
                std::to_string(atom.y),
                std::to_string(atom.z),
        });
    }
    //
    coord.section_var.set("", matrix_str);
    //std::cout << "subsys set finished!\n"; 
    return subsys;
}

void Cp2k::set_cdcloud(std::string partition = "free", int nodes = 1, int ntask = 24, std::string jobname = "atomsciflow_job", std::string stdout = "slurm.out", std::string stderr = "slurm.err") {
    this->run_params["partition"] = partition;
    this->run_params["jobname"] = jobname;
    this->run_params["nodes"] = std::to_string(nodes);
    this->run_params["ntask"] = std::to_string(ntask);
    this->run_params["stdout"] = stdout;
    this->run_params["stderr"] = stderr;
}


std::string Cp2k::gen_cdcloud_string(std::string inpname, std::string output, std::string cmd = "cp2k.psmp") {
    std::string out = "";
    out += "#!/bin/bash\n";
    //out += std::format("#SBATCH -p {}\n", this->run_params["partition"]);
    //out += std::format("#SBATCH -N {}\n", this->run_params["nodes"]);
    //out += std::format("#SBATCH -n {}\n", this->run_params["ntask"]);
    //out += std::format("#SBATCH -J {}\n", this->run_params["jobname"]);
    //out += std::format("#SBATCH -o {}\n", this->run_params["stdout"]);
    //out += std::format("#SBATCH -e {}\n", this->run_params["stderr"]);
    auto tmp1 = boost::format("#SBATCH -p %1%\n") % this->run_params["partition"];
    out += tmp1.str();
    auto tmp2 = boost::format("#SBATCH -N %1%\n") % this->run_params["nodes"];
    out += tmp2.str();
    auto tmp3 = boost::format("#SBATCH -n %1%\n") % this->run_params["ntask"];
    out += tmp3.str();
    auto tmp4 = boost::format("#SBATCH -J %1%\n") % this->run_params["jobname"];
    out += tmp4.str();
    auto tmp5 = boost::format("#SBATCH -o %1%\n") % this->run_params["stdout"];
    out += tmp5.str();
    auto tmp6 = boost::format("#SBATCH -e %1%\n") % this->run_params["stderr"];
    out += tmp6.str();
    out += "#\n";
    out += "export I_MPI_PMI_LIBRARY=/opt/gridview/slurm/lib/libpmi.so\n";
    out += "export FORT_BUFFERED=1\n";
    //out += std::format("srun --mpi=pmix_v3 {} -in {} | tee {}\n", cmd, inpname, output);
    auto tmp7 = boost::format("srun --mpi=pmix_v3 %1% -in %2% | tee %3%\n") % cmd % inpname % output;
    out += tmp7.str();
    return out;
}
    
void Cp2k::gen_cdcloud(std::string inpname, std::string output, std::string directory, std::string cmd = "cp2k.psmp") {
    std::ofstream outfile;
    //std::filesystem::path p_out = directory;
    boost::filesystem::path p_out = directory;
    p_out /= inpname;
    p_out.replace_extension(".slurm");
    outfile.open(p_out.string());
    
    outfile << this->gen_cdcloud_string(inpname, output, cmd);

    outfile.close();
}


} // namespace atomsciflow

