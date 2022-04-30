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

#ifndef ATOMSCIFLOW_CP2K_POST_POST_H_
#define ATOMSCIFLOW_CP2K_POST_POST_H_

#include <map>
#include <string>
#include <regex>
#include <boost/any.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace atomsciflow::cp2k::post {

namespace pt = boost::property_tree;
namespace fs = boost::filesystem;

class Post {
public:

    Post();

    virtual ~Post();

    virtual void read_lines(const std::string& filepath);
    virtual void read(const std::string& filepath);
    virtual void write(const std::string& directory);
    virtual void set_run(std::string key, std::string value);
    virtual void run(const std::string& directory);

    virtual void add_rule(const std::string& key, boost::any rule);

    void add_rule_type_1(const std::string& key, std::string pat1, std::string pat2);

    std::vector<std::string> lines;
    pt::ptree info;
    std::map<std::string, std::string> run_params;
    std::map<std::string, boost::any> rules;
}; 

} // namespace atomsciflow::cp2k::post

#endif // ATOMSCIFLOW_CP2K_POST_POST_H_
