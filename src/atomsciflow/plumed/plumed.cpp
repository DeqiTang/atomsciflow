
#include "atomsciflow/plumed/plumed.h"

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <sstream>

namespace atomsciflow::plumed {

Plumed::Plumed() {

}

Plumed::~Plumed() {
    
}

std::string Plumed::to_string() {
    std::ostringstream out;
    for (const auto& item : this->actions) {
        out << item.second->to_string() << "\n";
    }
    return out.str();
}

void Plumed::add_action(const std::string& name_label) {
    this->actions[name_label] = std::make_shared<Action>();
    std::vector<std::string> vec_str;
    boost::split(vec_str, name_label, boost::is_any_of(":"));
    this->actions[name_label]->label = vec_str[0];
    this->actions[name_label]->name = vec_str[1];
}

} // namespace atomsciflow::plumed
