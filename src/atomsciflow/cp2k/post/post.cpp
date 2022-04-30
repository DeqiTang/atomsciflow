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

#include "atomsciflow/cp2k/post/post.h"

#include <iostream>
#include <chrono>
#include <thread>

namespace atomsciflow::cp2k::post {

Post::Post() {
    this->set_run("post-dir", "post.dir");
    this->set_run("cp2k-out", "cp2k.out");
    this->set_run("output-json", "post-cp2k.json");
}

Post::~Post() {

}

void Post::read_lines(const std::string& filepath) {
    std::ifstream stream;
    stream.open(filepath);
    std::string line;

    while (std::getline(stream, line)) {
        this->lines.emplace_back(line);
    }
    stream.close();
}

void Post::read(const std::string& filepath) {
    this->read_lines(filepath);

    auto apply_rules = [&](const std::string& line) {
        for (auto& item : this->rules) {
            boost::any_cast<std::function<void(const std::string&)>>(item.second)(line);
        }        
    };

    auto start = std::chrono::system_clock::now();
    // for (const auto& line : lines) {
    //     apply_rules(line);
    // }
    auto apply_rules_partition = [&](int start, int end) {
        for (auto it = lines.begin() + start; it != lines.begin() + end; it++) {
            apply_rules(*it);
        }
    };    
    int nlines_each_task = this->lines.size() / 4;
    std::thread t1{apply_rules_partition, 0, 1*nlines_each_task};
    std::thread t2{apply_rules_partition, 1*nlines_each_task, 2*nlines_each_task};
    std::thread t3{apply_rules_partition, 2*nlines_each_task, 3*nlines_each_task};
    std::thread t4{apply_rules_partition, 3*nlines_each_task, 4*nlines_each_task};
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> time_consumed = end - start;
    std::cout << "Time to apply rules: " << time_consumed.count() << " seconds\n";
}

void Post::write(const std::string& directory) {
    pt::write_json((fs::path(directory) / run_params["output-json"]).string(), this->info);
}

void Post::set_run(std::string key, std::string value) {
    this->run_params[key] = value;
}

void Post::run(const std::string& directory) {
    this->read((fs::path(directory) / this->run_params["cp2k-out"]).string());
    fs::create_directory(fs::path(directory) / run_params["post-dir"]);
    this->write((fs::path(directory) / run_params["post-dir"]).string());
}

void Post::add_rule(const std::string& key, boost::any rule) {
    this->rules[key] = rule;
}

void Post::add_rule_type_1(const std::string& key, std::string pat1, std::string pat2) {
    this->add_rule(key, std::function<void(const std::string&)>{[&](const std::string& str) {
        std::regex re_pat1{pat1};
        std::regex re_pat2{pat2};
        std::smatch m1;
        std::smatch m2;
        if (std::regex_search(str, m1, re_pat1)) {
            std::regex_search(str, m2, re_pat2);
            this->info.put(m1.str(0), m2.str(0));
        }
    }});
}

} // namespace atomsciflow::cp2k::post