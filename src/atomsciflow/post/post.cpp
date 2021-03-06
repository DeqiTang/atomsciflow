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

#include "atomsciflow/post/post.h"

#include <iostream>
#include <chrono>
#include <thread>
#ifdef _OPENMP
#include <omp.h>
#endif // _OPENMP

namespace atomsciflow::post {

Post::Post() {
    this->set_run("post-dir", "post.dir");
    this->set_run("program-out", "program.out");
    this->set_run("output-json", "post-program.json");
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
    auto read_lines_start = std::chrono::system_clock::now();
    this->read_lines(filepath);
    std::chrono::duration<double> time_consumed = std::chrono::system_clock::now() - read_lines_start;
    std::cout << "Time for reading lines: " << time_consumed.count() << " seconds\n";

    auto apply_rules = [&](const std::string& line) {
        // #pragma omp parallel for
        // for (auto& item : this->rules) {
        //     boost::any_cast<std::function<void(const std::string&)>>(item)(line);
        // }
        auto apply_rules_partition = [&](int start, int end) {
            for (auto it = rules.begin() + start; it != rules.begin() + end; it++) {
                boost::any_cast<std::function<void(const std::string&)>>(*it)(line);
            }
        };
        int n_threads = 4;
        int n_rules = this->rules.size();
        int n_rules_each_task_base = n_rules / n_threads;
        int n_rules_remainder = n_rules % n_threads;
        std::vector<int> n_rules_each_task;
        for (int i = 0; i < n_threads; i++) {
            if (i < n_rules_remainder) {
                n_rules_each_task.push_back(n_rules_each_task_base + 1);
            } else {
                n_rules_each_task.push_back(n_rules_each_task_base);
            }
        }
        std::vector<std::thread> tasks;
        int rules_range = 0;
        for (int i = 0; i < n_threads; i++) {
            tasks.push_back(std::thread{
                apply_rules_partition,
                rules_range,
                rules_range + n_rules_each_task[i]
            });
            rules_range += n_rules_each_task[i];
        }
        for (auto& item : tasks) {
            item.join();
        }
    };

    auto apply_rules_start = std::chrono::system_clock::now();
    for (const auto& item : lines) {
        apply_rules(item);
    }
    time_consumed = std::chrono::system_clock::now() - apply_rules_start;
    std::cout << "Time to apply rules: " << time_consumed.count() << " seconds\n";
}

void Post::write(const std::string& directory) {
    pt::write_json((fs::path(directory) / run_params["output-json"]).string(), this->info);
}

void Post::set_run(std::string key, std::string value) {
    this->run_params[key] = value;
}

void Post::run(const std::string& directory) {
    this->read((fs::path(directory) / this->run_params["program-out"]).string());
    fs::create_directory(fs::path(directory) / run_params["post-dir"]);
    this->write((fs::path(directory) / run_params["post-dir"]).string());
}

void Post::add_rule(boost::any rule) {
    this->rules.emplace_back(rule);
}

void Post::add_rule_type_1(std::string pat1, std::string pat2) {
    this->add_rule(std::function<void(const std::string&)>{[&](const std::string& str) {
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

} // namespace atomsciflow::post
