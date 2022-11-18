//
// Created by Daniel St. John on 11/17/22.
//

#pragma once

#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

class TCPUtils {

private:
    boost::asio::io_service io_service;
    tcp::socket* m_socket;

public:
    TCPUtils();
    string read() ;
    void send(const string &message);
    virtual ~TCPUtils();
};


