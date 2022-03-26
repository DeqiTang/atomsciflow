/// @file src/atomsciflow/orca/block.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Sat 26 Mar 2022 03:16:14 PM CST

#include "atomsciflow/orca/block.h"

#include <sstream>
#include <boost/format.hpp>

namespace atomsciflow::orca {

Block::Block() {

}

Block::Block(const std::string& name) {
    this->name = name;
}

std::string Block::to_string() {
    std::ostringstream out;
    out << "%" << this->name << "\n";
    for (const auto& item : this->data) {
        out << item << "\n";
    }

    out << "end\n";
    return out.str();
}

} // namespace atomsciflow::orca
