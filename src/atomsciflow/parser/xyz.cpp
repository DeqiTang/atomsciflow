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

#include "atomsciflow/parser/xyz.h"

#include <iterator>
#include <boost/algorithm/string.hpp>

#include "atomsciflow/base/atom.h"

namespace atomsciflow {

int read_xyz_file(atomsciflow::Crystal* crystal, std::string filepath) {
    
    int i = 0; // for iteration
    int j = 0; // for iteration
    
    int natom = 0; // number of atoms each image

    std::ifstream xyzfile;
    xyzfile.open(filepath);
    std::string line;
    std::vector<std::string> lines;

    while (std::getline(xyzfile, line)) {
        // ignore empty lines
        std::istringstream iss(line);
        std::vector<std::string> line_split((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
        if (line_split.size() != 0) {
            lines.push_back(line);
        }
    }
    xyzfile.close();
    
    std::regex whitespace("\\s+");
    
    std::vector<std::string> line_split(std::sregex_token_iterator(lines[0].begin(), lines[0].end(), whitespace, -1), 
        std::sregex_token_iterator());
    natom = int(std::atof(line_split[0].c_str()));
    
    if (natom != (lines.size() - 2)) {
        std::cout <<  "Warning!!!" << " number of atoms specified in the first line is not equal to total line number of file minus 2(empty lines are removed)" << std::endl;
        std::exit(1);
    }
    
    std::vector<std::string> cell_line_split(std::sregex_token_iterator(lines[1].begin(), lines[1].end(), whitespace, -1), 
        std::sregex_token_iterator());
        
    std::vector<double> a;
    std::vector<double> b;
    std::vector<double> c;
    a.push_back(std::atof(cell_line_split[1].c_str()));
    a.push_back(std::atof(cell_line_split[2].c_str()));
    a.push_back(std::atof(cell_line_split[3].c_str()));
    b.push_back(std::atof(cell_line_split[5].c_str()));
    b.push_back(std::atof(cell_line_split[6].c_str()));
    b.push_back(std::atof(cell_line_split[7].c_str()));
    c.push_back(std::atof(cell_line_split[9].c_str()));
    c.push_back(std::atof(cell_line_split[10].c_str()));
    c.push_back(std::atof(cell_line_split[11].c_str()));
    crystal->cell.push_back(a);
    crystal->cell.push_back(b);
    crystal->cell.push_back(c);        

    std::vector<std::string> str_vec;

    for (i = 0; i < natom; i++) {
        std::string line = lines[i+2];
        line = boost::trim_left_copy(line);
        boost::split(str_vec, line, boost::is_space(), boost::token_compress_on);
        Atom atom;
        atom.set_name(str_vec[0]);
        atom.set_x(std::atof(str_vec[1].c_str()));
        atom.set_y(std::atof(str_vec[2].c_str()));
        atom.set_z(std::atof(str_vec[3].c_str()));
        crystal->atoms.push_back(atom);
    }    

    return 0;
}

int write_xyz_file(atomsciflow::Crystal* crystal, std::string filepath) {
    std::ofstream xyzfile;
    xyzfile.open(filepath);

    xyzfile.setf(std::ios::fixed);
    xyzfile << crystal->atoms.size() << "\n";
    
    xyzfile << "cell: "
        << std::setprecision(9) << std::setw(15) << crystal->cell[0][0] << " " 
        << std::setprecision(9) << std::setw(15) << crystal->cell[0][1] << " " 
        << std::setprecision(9) << std::setw(15) << crystal->cell[0][2] << " | " 
        << std::setprecision(9) << std::setw(15) << crystal->cell[1][0] << " " 
        << std::setprecision(9) << std::setw(15) << crystal->cell[1][1] << " " 
        << std::setprecision(9) << std::setw(15) << crystal->cell[1][2] << " | " 
        << std::setprecision(9) << std::setw(15) << crystal->cell[2][0] << " " 
        << std::setprecision(9) << std::setw(15) << crystal->cell[2][1] << " " 
        << std::setprecision(9) << std::setw(15) << crystal->cell[2][2] << "\n";
    
    for (auto& atom : crystal->atoms) {
        xyzfile <<
            atom.name << "\t"
            << std::setprecision(9) << std::setw(15) << atom.x << "\t"
            << std::setprecision(9) << std::setw(15) << atom.y << "\t"
            << std::setprecision(9) << std::setw(15) << atom.z << "\t" << "\n";
    }
    xyzfile.close();
    return 0;
}

} // end namespace atomsciflow
