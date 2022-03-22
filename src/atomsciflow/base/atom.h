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

#ifndef ATOMSCIFLOW_BASE_ATOM_H_
#define ATOMSCIFLOW_BASE_ATOM_H_

//#include "element.h"
#include <vector>
#include <string>

namespace atomsciflow {

class Atom {
  public:
    
    explicit Atom() {};

    ~Atom() {};

    std::string get_name(std::string name) { return this->name; }

    double get_x() { return this->x; }

    double get_y() { return this->y; }

    double get_z() { return this->z; }

    void set_name(std::string name) { this->name = name; }

    void set_x(double x) { 
        //this->cartesian[0] = x; 
        this->x = x;
    }

    void set_y(double y) {
        this->y = y;
    }

    void set_z(double z) {
        this->z = z;
    }

    void set_xyz(double x, double y, double z) {
        //this->cartesian[0] = x;
        //this->cartesian[1] = y;
        //this->cartesian[2] = z;
        this->x = x;
        this->y = y;
        this->z = z;
    }

    double x, y, z;
    std::string name;
    
  private:
    //double cartesian[3];
};


} //namespace atomsciflow

#endif // ATOMSCIFLOW_BASE_ATOM_H_
