//
// Created by Daniel St. John on 11/18/22.
//
#pragma once


/**
 * Creates get<funName>/set<funName> for instance varable of
 * type <varType> and name <varName> with initial value v
 *
 */
#define ATTR(varType, varName, funName, v)\
    protected: varType varName = v ;\
    public: virtual varType get##funName(void) const {\
        return varName;\
    }\
    public: virtual void set##funName(varType var){\
        varName = var;\
    }\

#define ATT(varType, varName, funName)\
    protected: varType varName ;\
    public: virtual varType get##funName(void) const {\
        return varName;\
    }\
    public: virtual void set##funName(varType var){\
        varName = var;\
    }                                 \

#include <boost/asio.hpp>
#include <string>
#include <sstream>
#include <exception>

using namespace boost;

namespace lm {
    namespace  spp {


        class EndPointException : public exception {
        public:
            EndPointException(boost::system::error_code ec) : m_ec(ec) {}

            virtual const char *what() const throw() {
                std::stringstream ss;
                ss << "code:" << ec.value() << "message:" << ec.message() << std::endl;
                return ss.str().c_str();
            }

        private:
            boost::system::error_code m_ec
        };


        class utils {

            static endpoint GetEndpoint(std::string host, unsigned short port) {
                boost::system::error_code ec;
                // checks whether argument contains a valid IPv4 or IPv6 address then
                // instantiates a corresponding v4 || v6
                asio::ip::address ip_address = asio::ip::address::from_string(host, ec);
                if (ec.value() != 0) {
                    throw new EndPointException(ec);
                }
                auto ep = endpoint(ip_address, port);
                return ep;
            }
        };
    }
}



