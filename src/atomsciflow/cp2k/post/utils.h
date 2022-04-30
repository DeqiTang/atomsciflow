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

#ifndef ATOMSCIFLOW_CP2K_POST_UTILS_H_
#define ATOMSCIFLOW_CP2K_POST_UTILS_H_

#include <regex>
#include <fstream>
#include <string>
#include <map>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace atomsciflow::cp2k::post {

namespace pt = boost::property_tree;

/**
 * @brief Get the info pat
 *  First, check whether pat1 is matched in str.
 *  Then, pat2 is used to extract the value.
 *  Finally, the matching result of pat1 and pat2
 *  are used as the key and value to set in this->info
 * @param info 
 * @param str 
 * @param pat1
 * @param pat2
 */
inline void get_info_pat(pt::ptree& info, const std::string& str, std::regex pat1, std::regex pat2) {
    std::smatch m1;
    std::smatch m2;
    if (std::regex_search(str, m1, pat1)) {
        std::regex_search(str, m2, pat2);
        info.put(m1.str(0), m2.str(0));
    }
}

} // namespace atomsciflow::cp2k::post

#endif // ATOMSCIFLOW_CP2K_POST_UTILS_H_
