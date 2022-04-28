#ifndef ATOMSCIFLOW_PLUMED_ACTION_H_
#define ATOMSCIFLOW_PLUMED_ACTION_H_

#include <string>
#include <vector>

namespace atomsciflow::plumed {

class Action {
public:

    Action();
    ~Action();

    std::string to_string();

    void add_keyword(const std::string& keyword);

    std::string label;
    std::string name;
    std::vector<std::string> keywords;
};

} // namespace atomsciflow::plumed

#endif // ATOMSCIFLOW_PLUMED_ACTION_H_