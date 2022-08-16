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

#include "atomsciflow/plumed/plumed.h"

#include <sstream>

namespace atomsciflow::plumed {

Plumed::Plumed() {

}

Plumed::~Plumed() {
    
}

std::string Plumed::to_string() {
    std::ostringstream out;
    for (const auto& key : this->actions_order) {
        out << this->actions[key]->to_string() << "\n";
    }
    return out.str();
}

/**
 * @brief 
 *
 * @param label_name 
 *  label_name could be in the following format:
 *      label:name
 *           :name
 *            name
 */
void Plumed::add_action(std::string label_name) {
    std::vector<std::string> str_vec;
    boost::split(str_vec, label_name, boost::is_any_of(":"), boost::token_compress_on);
    std::string key = boost::join(str_vec, ":");
    if (this->actions.find(key) != this->actions.end()) {
        return;
    }
    this->actions[key] = std::make_shared<Action>();
    this->actions[key]->label = str_vec[0];
    this->actions[key]->name = str_vec[1];
    this->actions_order.push_back(key);
}

} // namespace atomsciflow::plumed
