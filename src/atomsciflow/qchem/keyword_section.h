/// @file src/atomsciflow/qchem/keyword_section.h
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Sat 26 Mar 2022 04:28:11 PM CST

#ifndef ATOMSCIFLOW_QCHEM_KEYWORD_SECTION_H_
#define ATOMSCIFLOW_QCHEM_KEYWORD_SECTION_H_

#include <vector>
#include <string>

namespace atomsciflow::qchem {

class KeywordSection {
public:

    KeywordSection();
    explicit KeywordSection(const std::string& name);
    ~KeywordSection();

    std::string to_string();

    std::string name;
    std::vector<std::string> data;

};

} // namespace atomsciflow::qchem

#endif // ATOMSCIFLOW_QCHEM_KEYWORD_SECTION_H_