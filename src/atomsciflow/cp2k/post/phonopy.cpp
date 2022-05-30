/************************************************************************
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
************************************************************************/

#include "atomsciflow/cp2k/post/phonopy.h"

#include <yaml-cpp/yaml.h>

namespace atomsciflow::cp2k::post {

Phonopy::Phonopy() {

}

Phonopy::~Phonopy() {

}

void Phonopy::run(const std::string& directory) {

    // std::ifstream in;
    // in.open((fs::path(directory) / "phonopy-run.txt").string());
    // std::string xyz_file;
    // std::getline(in, xyz_file);
    // in.close();
    // xyz.read_xyz_file((fs::path(directory) / fs::path(xyz_file).filename()).string());

    YAML::Node phonopy_disp_yaml = YAML::LoadFile((fs::path(directory) / "phonopy_disp.yaml").string());
    auto dim = phonopy_disp_yaml["phonopy"]["configuration"]["dim"].as<std::string>();

    fs::create_directory(fs::path(directory) / run_params["post-dir"]);
    std::ofstream stream;
    
    stream.open((fs::path(directory) / run_params["post-dir"] / "mesh.conf").string());
    // stream << "ATOM_NAME =";
    // for (const auto& element : xyz.elements_set) {
    //     stream << " " << element;
    // }
    // stream << "\n";
    stream << "DIM = " << 1 << 1 << 1 << "\n";    
    stream.close();

    stream.open((fs::path(directory) / run_params["post-dir"] / "pdos.conf").string());
    // stream << "ATOM_NAME =";
    // for (const auto& element : xyz.elements_set) {
    //     stream << " " << element;
    // }
    // stream << "\n";    
    stream << "DIM = " << 1 << 1 << 1 << "\n";
    stream.close();

    stream.open((fs::path(directory) / run_params["post-dir"] / "band.conf").string());
    // stream << "ATOM_NAME =";
    // for (const auto& element : xyz.elements_set) {
    //     stream << " " << element;
    // }
    // stream << "\n";    
    stream << "DIM = " << dim << "\n";
    stream.close();

    stream.open((fs::path(directory) / run_params["post-dir"] / "analysis.sh").string());
    stream << "#!/bin/bash\n\n"
        << "#\n"
        << boost::format("cd %1%\n") % (fs::absolute(fs::path(directory)) / run_params["post-dir"]).string()
        << "\n"
        << "cp ../phonopy_disp.yaml ./\n"
        << "# generate the FORCE_SETS\n"
        << "phonopy --cp2k -f ../cp2k-supercell-{001..032}-forces-1_0.xyz\n"
        << "# plot the phonon dos\n"
        << "phonopy --cp2k -p ../mesh.conf -c ../cp2k.inp\n"
        << "# calclate thermal properties with specified sampling mesh\n"
        << "phonopy --cp2k -t ../mesh.conf -c ../cp2k.inp\n"
        << "# calculate projected phonon dos\n"
        << "phonopy --cp2k -p ../dos.conf -c ../cp2k.inp\n"
        << "# calculate phonon band\n"
        << "phonopy --cp2k -p ../band.conf -c ../cp2k.inp\n"
        ;
    stream.close();

    std::string cmd = "";
    cmd += "bash ";
    cmd += (fs::path(directory) / run_params["post-dir"] / "analysis.sh").string();
    std::system(cmd.c_str());

}

} // namespace atomsciflow::cp2k::post
