#include "atomsciflow/plumed/action.h"

#include <boost/format.hpp>
#include <iostream>
#include <sstream>

namespace atomsciflow::plumed {

Action::Action() {
    this->label = "";
}

Action::~Action() {

}

std::string Action::to_string() {
    std::ostringstream out;
    if ("" != this->label) {
        out << boost::format("%1%: %2%") % this->label % this->name;
    } else {
        out << boost::format("%1%") % this->name;
    }
    for (const auto& item : this->keywords) {
        out << boost::format(" %1%") % item;
    }
    return out.str();
}

void Action::add_keyword(const std::string& keyword) {
    this->keywords.push_back(keyword);
}

} // namespace atomsciflow::plumed