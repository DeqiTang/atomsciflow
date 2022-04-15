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

#ifndef ATOMSCIFLOW_CP2K_POST_OPT_H_
#define ATOMSCIFLOW_CP2K_POST_OPT_H_

#include <string>
#include <map>

namespace atomsciflow::cp2k::post {

class Opt {
public:

    Opt();
    ~Opt();

    void read(const std::string& filepath);
    void set_run(std::string key, std::string value);
    void run(const std::string& directory);

    std::map<std::string, std::string> info;
    std::map<std::string, std::string> run_params;
};

} // namespace atomsciflow::post

#endif // ATOMSCIFLOW_CP2K_POST_OPT_H_
