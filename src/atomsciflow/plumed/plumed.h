#ifndef ATOMSCIFLOW_PLUMED_PLUMED_H_
#define ATOMSCIFLOW_PLUMED_PLUMED_H_

#include <map>
#include <memory>

#include "atomsciflow/plumed/action.h"

namespace atomsciflow::plumed {

class Plumed {
public:

    Plumed();
    ~Plumed();

    std::string to_string();

    void add_action(const std::string& name_label);

    /**
     * map of string to Action,
     * the string is in format:
     *  {{label}}:{{name}}
     */
    std::map<std::string, std::shared_ptr<Action>> actions;
};

} // namespace atomsciflow::plumed

#endif // ATOMSCIFLOW_PLUMED_PLUMED_H_