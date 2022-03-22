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

/// @file src/atomsciflow/remote/ssh.h
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Sun 23 Jan 2022 04:08:25 PM CST

#ifndef ATOMSCIFLOW_REMOTE_SSH_
#define ATOMSCIFLOW_REMOTE_SSH_

#include <libssh2.h>
#include <boost/asio.hpp>
#include <string>

namespace atomsciflow {

class Ssh {
public:
    Ssh(); 
    ~Ssh(); 

    void get_server_config();
    void get_server_info(const std::string& ip, const std::string& username, const std::string& password);

    void connect();
    void execute(const std::string& command);

    boost::asio::io_service _io_service;
    boost::asio::ip::tcp::socket sock;
    LIBSSH2_SESSION* session;
    LIBSSH2_CHANNEL* channel;
    LIBSSH2_KNOWNHOSTS* knownhosts;

    std::string ip;
    static int port; // 22
    std::string username;
    std::string password;
};

// initialize static member variable
//inline int Ssh::port = 22;

} // namespace atomsciflow

#endif // ATOMSCIFLOW_REMOTE_SSH_
