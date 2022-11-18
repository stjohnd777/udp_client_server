//
// Created by Daniel St. John on 11/18/22.
//
#pragma once

#include <boost/asio.hpp>
#include <string>
#include <sstream>
#include <exception>

using namespace boost;

class EndPointException : public exception {
public:
    EndPointException(boost::system::error_code ec) : m_ec(ec){ }
    virtual const char * what () const throw () {
        std::stringstream ss;
        ss << "code:" <<  ec.value() <<  "message:" <<  ec.message() << std::endl;
        return ss.str().c_str();
    }
private:
    boost::system::error_code m_ec
};

using namespace asio::ip::tcp;
class utils {

    static  endpoint GetEndpoint(std::string host, unsigned short port){
        boost::system::error_code ec;
        // checks whether argument contains a valid IPv4 or IPv6 address then
        // instantiates a corresponding v4 || v6
        asio::ip::address ip_address = asio::ip::address::from_string(host, ec);
        if (ec.value() != 0) {
            throw new  EndPointException(ec);
        }
        auto ep =  endpoint(ip_address, port);
        return ep;
    }
};



