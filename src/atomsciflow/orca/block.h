/// @file src/atomsciflow/orca/block.h
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Sat 26 Mar 2022 03:16:12 PM CST

#ifndef ATOMSCIFLOW_ORCA_BLOCK_H_
#define ATOMSCIFLOW_ORCA_BLOCK_H_

#include <vector>
#include <string>

namespace atomsciflow::orca {

class Block {
public:

    Block();
    explicit Block(const std::string& name);

    std::string to_string();

    std::string name;
    std::vector<std::string> data;

};

} // namespace atomsciflow::orca

#endif // ATOMSCIFLOW_ORCA_BLOCK_H_
