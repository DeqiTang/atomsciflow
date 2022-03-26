/// @file src/atomsciflow/qchem/keyword_section.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Sat 26 Mar 2022 04:28:15 PM CST

#include "atomsciflow/qchem/keyword_section.h"

#include <sstream>
#include <boost/format.hpp>

namespace atomsciflow::qchem {

KeywordSection::KeywordSection() {

}

KeywordSection::~KeywordSection() {

}

KeywordSection::KeywordSection(const std::string& name) {
    this->name = name;
}

std::string KeywordSection::to_string() {
    std::ostringstream out;
    out << "$" << this->name << "\n";
    for (const auto& item : this->data) {
        out << item << "\n";
    }

    out << "$end\n";
    return out.str();
}

} // namespace atomsciflow::qchem
