/************************************************************************
MIT License

Copyright (c) 2021 Deqi Tang

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

/// @file src/atomsciflow/qmcpack/input.h
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Sun 13 Mar 2022 03:26:14 PM CST

#ifndef ATOMSCIFLOW_QMCPACK_INPUT_H_
#define ATOMSCIFLOW_QMCPACK_INPUT_H_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <iostream>

namespace atomsciflow::qmcpack {

namespace pt = boost::property_tree;

class Input {
public:

    Input();
    ~Input();

    template<typename T>
    void put_i_key(const std::string& path, int i, const std::string& key, const T& value);

    template<typename T>
    void put_i_key_attr(const std::string& path, int i, const std::string& key, const std::string& attr, const T& value);

    template<typename T>
    void put_attr(const std::string& path, const std::string& attr, const T& value);

    std::string to_string();

    pt::ptree root;
};

/**
 * @brief Input::put_i_key
 * @param path
 * @param key
 * @param value
 * @param i
 * circumstance:
 *  there are multi child with the key under path.
 *  call put_value on the i-th key of the parent path
 */
template<typename T>
void Input::put_i_key(const std::string& path, int i, const std::string& key, const T& value) {

    // if path doens't exist, generate it
    if (this->root.get_child_optional(path) == boost::none) {
        this->root.put(path, "");
    }

    // if number of path.key is less than (i+1), generate them
    int n_key = 0;
    for (const auto& item : this->root.get_child(path)) {
        if (item.first == key) {
            n_key += 1;
        }
    }
    int plus_n_key = (i + 1) - n_key;
    while (plus_n_key > 0) {
        this->root.add(path + "." + key, "");
        plus_n_key -= 1;
    }

    // get the iterator of the i-th key in path
    int n = i;
    auto it = this->root.get_child(path).begin();
    if (it->first == "<xmlattr>") {
        // if path.<xml.attr> is set previously
        // the path begin() will be the <xmlattr>
        // otherwise it will be formal xml label.
        ++it;
    }
    while (n > 0) {
        if (it->first == key) {
            n -= 1;
        }
        ++it;
    }
    it->second.put_value(value);
}

/**
 * @brief put_i_key_attr
 * @param path
 * @param key
 * @param attr
 * @param value
 * @param i
 * circumstance:
 *  there are multi child with the key under path.
 *  call put_value on the attr of the i-th key of the parent path
 */
template<typename T>
void Input::put_i_key_attr(const std::string& path, int i, const std::string& key, const std::string& attr, const T& value) {

    // if path doens't exist, generate it
    if (this->root.get_child_optional(path) == boost::none) {
        this->root.put(path, "");
    }

    // if number of path.key is less than (i+1), generate them
    int n_key = 0;
    for (const auto& item : this->root.get_child(path)) {
        if (item.first == key) {
            n_key += 1;
        }
    }
    int plus_n_key = (i + 1) - n_key;
    while (plus_n_key > 0) {
        this->root.add(path + "." + key, "");
        plus_n_key -= 1;
    }

    // get the iterator of the i-th key in path
    int n = i;
    auto it = this->root.get_child(path).begin();
    if (it->first == "<xmlattr>") {
        // if path.<xml.attr> is set previously
        // the path begin() will be the <xmlattr>
        // otherwise it will be formal xml label.
        ++it;
    }
    while (n > 0) {
        if (it->first == key) {
            n -= 1;
        }
        ++it;
    }
    it->second.put(std::string("<xmlattr>.") + attr, value);
}

/**
 * @brief Input::put_attr
 * @param path
 * @param attr
 * @param value
 * preliminaries:
 *  there is only one object with the path. For example,
 *  only one c in path a.b.c
 */
template<typename T>
void Input::put_attr(const std::string& path, const std::string& attr, const T& value) {
    this->root.put(path + ".<xmlattr>." + attr, value);
}

} // namespace atomsciflow::qmcpack

#endif // ATOMSCIFLOW_QMCPACK_INPUT_H_
