// @file src/atomsciflow/remote/ssh.cpp
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Sat 12 Feb 2022 10:18:05 PM CST

#include "atomsciflow/remote/ssh.h"

namespace atomsciflow {


Ssh::Ssh() : sock(_io_service) {
    libssh2_init(0); 

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


void Ssh::connect(const std::string& hostname, const std::string& username, const std::string& password) {

    // connect socket

    // create ssh session

    // authentication

}

void Ssh::execute(const std::string& command) {

}



} // namespace atomsciflow

