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

/// @file src/atomsciflow/nwchem/nwchem.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com 
/// Created Time: Fri 25 Mar 2022 03:28:12 PM CST

#include "atomsciflow/nwchem/nwchem.h"

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <iomanip>

namespace atomsciflow::nwchem {

namespace fs = boost::filesystem;

NWChem::NWChem() {

    set_keywords("task[0]", std::vector<std::string>{"scf", "energy"});

    add_keyword("charge[0]", 0);
    add_keyword("title[0]", "NWChem calculation");
    // add_keyword("geometry[0]/symmetry", "print");

    job.set_run("cmd", "$ASF_CMD_NWCHEM");
    job.set_run("script_name_head", "nwchem-run");    
}

NWChem::~NWChem() {

}

// task directive must be specified in the end
std::string NWChem::to_string() {
    std::ostringstream out;
    
    this->set_simple_default();

    int i = -1;
    for (auto& task : this->tasks) {
        i++;
        out << "#======================================================================#\n";
        out << "# TASK -> " << i << "\n";
        out << "#======================================================================#\n";
        for (auto& item : task) {
            if ("task" == item.first) {
                continue;
            }
            if ("title" == item.first) {
                out << "title \"";
                auto length = item.second->keywords.size();
                for (int i = 0; i < (length - 1); ++i) {
                    out << item.second->keywords[i] << " ";
                }
                out << item.second->keywords[length-1];
                out << "\"\n\n";
                continue;
            }
            out << item.second->to_string() << "\n";
        }
        out << task["task"]->to_string();
    }
    return out.str();
}

/**
 * @brief 
 * e.g.
 *  new_directive("task[0]");
 *  new_directive("title[0]");
 *  new_directive("geometry[0]/symmetry");
 *  new_directive("geometry@xxx[0]/symmetry");
 * @param path 
 */
void NWChem::new_directive(const std::string& path) {
    std::vector<std::string> str_vec_1;
    std::vector<std::string> str_vec_2;
    boost::split(str_vec_1, path, boost::is_any_of("["), boost::token_compress_on);
    boost::split(str_vec_2, str_vec_1[1], boost::is_any_of("]"), boost::token_compress_on);
    int i_task = boost::lexical_cast<int>(str_vec_2[0]);
    
    int num_tasks = this->tasks.size();
    while (i_task > num_tasks - 1) {
        std::map<std::string, std::shared_ptr<nwchem::Directive>> task;
        this->tasks.push_back(task);
        num_tasks++;
    }

    boost::split(str_vec_2, path, boost::is_any_of("/"), boost::token_compress_on);
    str_vec_2[0] = str_vec_1[0];
    int length = str_vec_2.size();

    auto new_recursive = [&] (auto&& _new_recursive, std::map<std::string, std::shared_ptr<Directive>>& _direcs, int i) {
        std::string direc_key = "";
        direc_key = str_vec_2[i];
        if (_direcs.find(direc_key) == _direcs.end()) {
            std::vector<std::string> str_vec;
            boost::split(str_vec, direc_key, boost::is_any_of("@"), boost::token_compress_on);
            _direcs[direc_key] = std::make_shared<Directive>(str_vec[0]);
        }

        if (i + 1 >= length) {
            return;
        }
        _new_recursive(_new_recursive, _direcs[direc_key]->directives, i + 1);
    };
    new_recursive(new_recursive, this->tasks[i_task], 0);
}

/**
 * @brief 
 * e.g.
 *  add_keyword("task[0]", "dft");
 *  add_keyword("title[0]", "NWChem-calculation");
 *  add_keyword("geometry[0]/symmetry", "print");
 *  add_keyword("geometry@xxx[0]/symmetry", "print");
 * @tparam T 
 * @param path 
 * @param keyword 
 */
template <typename T>
void NWChem::add_keyword(const std::string& path, T keyword) {
    this->new_directive(path);

    std::vector<std::string> str_vec_1;
    std::vector<std::string> str_vec_2;
    boost::split(str_vec_1, path, boost::is_any_of("["), boost::token_compress_on);
    boost::split(str_vec_2, str_vec_1[1], boost::is_any_of("]"), boost::token_compress_on);
    int i_task = boost::lexical_cast<int>(str_vec_2[0]);

    boost::split(str_vec_2, path, boost::is_any_of("/"), boost::token_compress_on);
    str_vec_2[0] = str_vec_1[0];
    int length = str_vec_2.size();

    auto add_recursive = [&] (auto&& _add_recursive, std::map<std::string, std::shared_ptr<Directive>>& _direcs, int i) {
        std::string direc_key = "";
        direc_key = str_vec_2[i];

        if (i + 1  == length) {
            _direcs[direc_key]->keywords.push_back(boost::lexical_cast<std::string>(keyword));
            return;
        }
        _add_recursive(_add_recursive, _direcs[direc_key]->directives, i + 1);
    };
    add_recursive(add_recursive, this->tasks[i_task], 0);
}

/**
 * Sometimes in NWChem, only two keywords make sense, like "units angstroms"
 * Thus, we provide an overloaded template here.
 * Note that usually the keyword_1 in such case is usually a string, and 
 * when build the instantiation of the template we only sepcify std::string
 * for keyword_1 type.
 */
template <typename T, typename U>
void NWChem::add_keyword(const std::string& path, T keyword_1, U keyword_2) {
    this->add_keyword(path, keyword_1);
    this->add_keyword(path, keyword_2);
}

void NWChem::py_add_keyword(const std::string& path, int keyword) {
    this->add_keyword(path, keyword);
}

void NWChem::py_add_keyword(const std::string& path, double keyword) {
    this->add_keyword(path, keyword);
}

void NWChem::py_add_keyword(const std::string& path, std::string keyword) {
    this->add_keyword(path, keyword);
}

void NWChem::py_add_keyword(const std::string& path, int keyword_1, int keyword_2) {
    this->add_keyword(path, keyword_1);
    this->add_keyword(path, keyword_2);
}

void NWChem::py_add_keyword(const std::string& path, double keyword_1, double keyword_2) {
    this->add_keyword(path, keyword_1);
    this->add_keyword(path, keyword_2);
}

void NWChem::py_add_keyword(const std::string& path, std::string keyword_1, std::string keyword_2) {
    this->add_keyword(path, keyword_1);
    this->add_keyword(path, keyword_2);
}

void NWChem::py_set_field(const std::string& path, int field, int row, int col) {
    this->set_field(path, field, row, col);
}

void NWChem::py_set_field(const std::string& path, double field, int row, int col) {
    this->set_field(path, field, row, col);
}

void NWChem::py_set_field(const std::string& path, std::string field, int row, int col) {
    this->set_field(path, field, row, col);
}

void NWChem::get_xyz(const std::string& filepath) {
    this->xyz.read_xyz_file(filepath);
    job.set_run("xyz_file", fs::absolute(filepath).string());

    this->set_keywords("geometry[0]", std::vector<std::string>{"units", "angstroms"});
    int i = 0;
    for (const auto& item : this->xyz.atoms) {
        this->set_field("geometry[0]", item.name, i, 0);
        this->set_field("geometry[0]", (boost::format("%1$10.8f") % item.x).str(), i, 1);
        this->set_field("geometry[0]", (boost::format("%1$10.8f") % item.y).str(), i, 2);
        this->set_field("geometry[0]", (boost::format("%1$10.8f") % item.z).str(), i, 3);
        i++;
    }

    for (const auto& item : this->xyz.elements_set) {
        this->set_keywords(
            (boost::format("basis[0]/%1%")%item).str(),
            std::vector<std::string>{"library", "6-31g"}
        );
        this->set_simple(
            (boost::format("basis[0]/%1%")%item).str(), 
            true
        );
    }
}

/**
 * @brief 
 * e.g.
 *  set_simple("task[0]", true);
 *  set_simple("title[0]", true);
 *  set_simple("geometry[0]/symmetry", true);
 *  set_simple("geometry@xxx[0]/symmetry", true);
 * @param path 
 * @param simple
 */
void NWChem::set_simple(const std::string& path, bool simple) {
    this->new_directive(path);

    std::vector<std::string> str_vec_1;
    std::vector<std::string> str_vec_2;
    boost::split(str_vec_1, path, boost::is_any_of("["), boost::token_compress_on);
    boost::split(str_vec_2, str_vec_1[1], boost::is_any_of("]"), boost::token_compress_on);
    int i_task = boost::lexical_cast<int>(str_vec_2[0]);

    boost::split(str_vec_2, path, boost::is_any_of("/"), boost::token_compress_on);
    str_vec_2[0] = str_vec_1[0];
    int length = str_vec_2.size();

    auto simple_recursive = [&] (auto&& _simple_recursive, std::map<std::string, std::shared_ptr<Directive>>& _direcs, int i) {
        std::string direc_key = "";
        direc_key = str_vec_2[i];

        if (i + 1  == length) {
            _direcs[direc_key]->simple = simple;
            return;
        }
        _simple_recursive(_simple_recursive, _direcs[direc_key]->directives, i + 1);
    };
    simple_recursive(simple_recursive, this->tasks[i_task], 0);

}

void NWChem::set_simple_default() {
    std::vector<std::string> paths;
    paths.push_back("start[*]");
    paths.push_back("title[*]");
    paths.push_back("task[*]");
    paths.push_back("charge[*]");
    paths.push_back("geometry[*]/symmetry");
    paths.push_back("mp2[*]/print");
    paths.push_back("scf[*]/print");
    paths.push_back("dft[*]/iterations");
    paths.push_back("dft[*]/print");
    paths.push_back("dft[*]/xc");
    paths.push_back("dft[*]/convergence");
    paths.push_back("driver[*]/print");
    paths.push_back("driver[*]/maxiter");
    paths.push_back("driver[*]/gmax");
    paths.push_back("driver[*]/grms");
    paths.push_back("driver[*]/xmax");
    paths.push_back("driver[*]/xrms");
    paths.push_back("driver[*]/eprec");
    paths.push_back("driver[*]/trust");
    paths.push_back("driver[*]/sadstp");
    paths.push_back("driver[*]/xyz");
    paths.push_back("freq[*]/reuse");
    paths.push_back("freq[*]/animate");
    paths.push_back("neb[*]/maxiter");
    paths.push_back("neb[*]/stepsize");
    paths.push_back("neb[*]/algorithm");
    paths.push_back("neb[*]/xyz_path");
    paths.push_back("neb[*]/nbeads");
    paths.push_back("neb[*]/kbeads");
    paths.push_back("neb[*]/gmax");
    paths.push_back("neb[*]/grms");
    paths.push_back("neb[*]/xmax");
    paths.push_back("neb[*]/xrms");
    paths.push_back("property[*]/all");
    paths.push_back("property[*]/response");
    paths.push_back("property[*]/velocity");
    paths.push_back("property[*]/orbeta");
    paths.push_back("property[*]/giao");
    paths.push_back("property[*]/bdtensor");
    paths.push_back("property[*]/analysis");
    paths.push_back("property[*]/damping");
    paths.push_back("property[*]/convergence");
    paths.push_back("property[*]/spinspin");
    paths.push_back("property[*]/gshift");
    paths.push_back("property[*]/efield");
    paths.push_back("property[*]/esp");
    paths.push_back("property[*]/octupole");
    paths.push_back("property[*]/quadrupole");
    paths.push_back("property[*]/dipole");
    paths.push_back("property[*]/mulliken");
    paths.push_back("property[*]/electrondensity");
    paths.push_back("property[*]/hyperfine");
    paths.push_back("property[*]/shielding");
    paths.push_back("property[*]/vectors");
    paths.push_back("property[*]/aimfile");
    paths.push_back("property[*]/nbofile");
    paths.push_back("property[*]/moldenfile");
    paths.push_back("property[*]/molden_norm");

    int n_tasks = int(this->tasks.size());
    for (int i = 0; i < n_tasks; i++) {
        for (const auto& path : paths) {
            std::vector<std::string> str_vec_1;
            std::vector<std::string> str_vec_2;
            boost::split(str_vec_1, path, boost::is_any_of("["), boost::token_compress_on);
            int i_task = i;

            boost::split(str_vec_2, path, boost::is_any_of("/"), boost::token_compress_on);
            str_vec_2[0] = str_vec_1[0];
            int length = str_vec_2.size();

            auto simple_recursive = [&] (auto&& _simple_recursive, std::map<std::string, std::shared_ptr<Directive>>& _direcs, int i) {
                std::string direc_key = "";
                direc_key = str_vec_2[i];
                if (_direcs.find(direc_key) == _direcs.end()) {
                    return;
                }
                if (i + 1  == length) {
                    _direcs[direc_key]->simple = true;
                    return;
                }
                _simple_recursive(_simple_recursive, _direcs[direc_key]->directives, i + 1);
            };
            simple_recursive(simple_recursive, this->tasks[i_task], 0);
        }
    }
}

/**
 * @brief 
 * e.g.
 *  set_status("task[0]", true);
 *  set_status("title[0]", true);
 *  set_status("geometry[0]/symmetry", true);
 *  set_status("geometry@xxx[0]/symmetry", true);
 * @param path 
 * @param status 
 */
void NWChem::set_status(const std::string& path, bool status) {
    this->new_directive(path);

    std::vector<std::string> str_vec_1;
    std::vector<std::string> str_vec_2;
    boost::split(str_vec_1, path, boost::is_any_of("["), boost::token_compress_on);
    boost::split(str_vec_2, str_vec_1[1], boost::is_any_of("]"), boost::token_compress_on);
    int i_task = boost::lexical_cast<int>(str_vec_2[0]);

    boost::split(str_vec_2, path, boost::is_any_of("/"), boost::token_compress_on);
    str_vec_2[0] = str_vec_1[0];
    int length = str_vec_2.size();

    auto status_recursive = [&] (auto&& _status_recursive, std::map<std::string, std::shared_ptr<Directive>>& _direcs, int i) {
        std::string direc_key = "";
        direc_key = str_vec_2[i];

        if (i + 1  == length) {
            _direcs[direc_key]->status = status;
            return;
        }
        _status_recursive(_status_recursive, _direcs[direc_key]->directives, i + 1);
    };
    status_recursive(status_recursive, this->tasks[i_task], 0);
}

void NWChem::run(const std::string& directory) {    
    std::ostringstream step;
    step << "cd ${ABSOLUTE_WORK_DIR}" << "\n";
    step << "cat > nwchem.nw<<EOF\n";
    step << this->to_string();
    step << "EOF\n";
    step << boost::format("$CMD_HEAD %1% nwchem.nw > nwchem.out\n")
        % job.run_params["cmd"];
    job.steps.push_back(step.str());
    step.clear();

    job.run(directory);
}

// explicit template instantiation
template void NWChem::add_keyword<int>(const std::string&, int);
template void NWChem::add_keyword<double>(const std::string&, double);
template void NWChem::add_keyword<std::string>(const std::string&, std::string);
template void NWChem::add_keyword<int, int>(const std::string&, int, int);
template void NWChem::add_keyword<double, double>(const std::string&, double, double);
template void NWChem::add_keyword<std::string, std::string>(const std::string&, std::string, std::string);

} // namespace atomsciflow::nwchem
