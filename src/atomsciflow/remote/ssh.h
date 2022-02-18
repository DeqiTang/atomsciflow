// @file src/atomsciflow/remote/ssh.h
// @author: DeqiTang
// Mail: deqitang@gmail.com 
// Created Time: Sun 23 Jan 2022 04:08:25 PM CST


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

    void connect(const std::string& hostname, const std::string& username, const std::string& password);
    void execute(const std::string& command);

    boost::asio::io_service _io_service;
    boost::asio::ip::tcp::socket sock;
    LIBSSH2_SESSION* session;
    LIBSSH2_CHANNEL* channel;
    LIBSSH2_KNOWNHOSTS* knownhosts;

    std::string username;
    std::string password;

};


} // namespace atomsciflow

#endif // ATOMSCIFLOW_REMOTE_SSH_
