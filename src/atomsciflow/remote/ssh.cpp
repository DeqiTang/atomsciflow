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

/// @file src/atomsciflow/remote/ssh.cpp
/// @author DeqiTang
/// Mail: deqitang@gmail.com
/// Created Time: Sat 12 Feb 2022 10:18:05 PM CST

#include "atomsciflow/remote/ssh.h"

#include <boost/lexical_cast.hpp>

#include "atomsciflow/config/config_manager.h"

namespace atomsciflow {

// initialize static member variable
int Ssh::port = 22;

Ssh::Ssh() : sock(_io_service) {
    libssh2_init(0);
    ip = "";
    username = "";
    password = "";
    session = nullptr;
    channel = nullptr;
}

Ssh::~Ssh() {
    if (this->channel != nullptr) {
        libssh2_channel_free(this->channel);
        this->channel = nullptr;
    }
    if (this->session != nullptr) {
        libssh2_session_set_blocking(this->session, 1);
        libssh2_session_disconnect(this->session, "disconnecting now");
        libssh2_session_free(this->session);
        this->session = nullptr;
    }
    try {
        boost::system::error_code err_code;
        sock.shutdown(boost::asio::ip::tcp::socket::shutdown_both, err_code);
        sock.close(err_code);
    } catch (std::exception& ex) {
       // doing nothing now
    }
    libssh2_exit();
}

void Ssh::get_server_config() {
    // get config info
    auto config = new ConfigManager();
    config->get_server_info();

    ip = config->server_json.get<std::string>("pbs.ip");
    username = config->server_json.get<std::string>("pbs.username");
    password = config->server_json.get<std::string>("pbs.password");

    delete config;

    return;
}

void Ssh::get_server_info(const std::string& ip, const std::string& username, const std::string& password) {
    this->ip = ip;
    this->username = username;
    this->password = password;

    return;
}

void Ssh::connect() {
    // connect socket
    boost::asio::ip::tcp::resolver resolver(this->_io_service);
    boost::asio::ip::tcp::resolver::query query{
                this->ip,
                //boost::lexical_cast<std::string>(this->port)
                std::to_string(this->port)
    };
    boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);
    boost::asio::connect(this->sock, iter);

    // create ssh session
    this->session = libssh2_session_init();
    libssh2_session_set_blocking(this->session, 0);

    // session handshake
    libssh2_session_handshake(this->session, sock.native_handle());

    // authentication
    libssh2_userauth_password(this->session, username.c_str(), password.c_str());

    return;
}

void Ssh::execute(const std::string& command) {

}

} // namespace atomsciflow
