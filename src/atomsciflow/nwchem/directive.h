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

/// @file src/atomsciflow/nwchem/directive.h
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Sat 26 Mar 2022 02:10:13 PM CST

#ifndef ATOMSCIFLOW_NWCHEM_DIRECTIVE_H_
#define ATOMSCIFLOW_NWCHEM_DIRECTIVE_H_

#include <map>
#include <vector>
#include <string>
#include <memory>

namespace atomsciflow::nwchem {

/**
 * There are two types of directive, i.e. simple or compound.
 * A simple directie consists of the name and keywords, with
 * the keywords being a string of token or fields, separated 
 * with whitespace, in format like this,
 *  
 *  NAME KEYWORD_1 KEYWORD_2 ... KEYWORD_N
 * 
 * A compound directive consists of the name, keywords and,
 * unlike simple directive, multiline fields and embedded
 * simple or compound directives, in format like this,
 * 
 *  NAME KEYWORD_1 KEYWORD_2 ... KEYWORD_N
 *      FIELDS_1_1 FIELDS_1_2 ... FIELDS_1_N
 *      FIELDS_2_1 FIELDS_2_2 ... FIELDS_2_N
 *      .
 *      .
 *      FIELDS_M_1 FIELDS_M_2 ... FIELDS_M_N
 *      
 *      COMPOUND_DIRECTIVE
 * 
 *      SIMPLE_DIRECTIVE
 *  END
 * 
 * Both simple directive and compound directiev are represented
 * with atomsciflow::nwchem::Directive. The logical member variable 
 * simple is used to differentiate them.
 * 
 */ 
class Directive {
public:

    Directive();
    explicit Directive(const std::string& name);

    ~Directive();

    std::string to_string();

    void set_keyworkds(std::vector<std::string>& keywords) {
        this->keywords = keywords;
    }

    std::string name;
    std::vector<std::string> keywords;
    bool simple;
    bool status;
    std::vector<std::vector<std::string>> fields;
    std::map<std::string, std::shared_ptr<Directive>> directives;
};

} // namespace atomsciflow::nwchem

#endif // ATOMSCIFLOW_NWCHEM_DIRECTIVE_H_