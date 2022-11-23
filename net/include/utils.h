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
#include <iostream>
#include <sstream>
#include <exception>
#include <memory>
#include <tuple>

using namespace boost;

namespace lm {
    namespace  spp {

        // TODO: maybe shared_ptr
        template <class SomeStruct>
        char*  Serialize(SomeStruct& s) {
            size_t size = sizeof(SomeStruct);
            char* p = (char*)&s;
            return p;
        }

        // TODO: maybe shared_ptr
        template <class SomeStruct>
        SomeStruct* DeSerialize( char* src) {
            size_t len = sizeof(SomeStruct);
            char* dest = new char[len];
            memcpy(dest, src, len);
            SomeStruct* s = (SomeStruct*)(dest);
            return s;
        }

        //template <class SomeStruct>
        //std::tuple<size_t,  char*  > _serialize(SomeStruct& s) {
        //    size_t size = sizeof(SomeStruct);
        //    char* p = (char*) &s;
        //    std::tuple<size_t,char*> t =  std::make_tuple(size,p);
        //    return t;
        //}
      
        //template <class SomeStruct>
        //std::tuple<size_t, std::shared_ptr <char*> > serialize(SomeStruct& s) {
        //    size_t size = sizeof(SomeStruct);
        //    std::shared_ptr<char*> sp = std::make_shared <char*> (&s);
        //    return std::make_tuple(size, sp);
        //}


        //template <class SomeStruct>
        //SomeStruct* _deserialize(std::tuple<size_t, char*> t) {
        //    auto len = std::get<0>(t);
        //    auto src = std::get<1>(t);
        //    auto dest = new char[len];
        //    memcpy(dest, src, len);
        //    SomeStruct* s = (SomeStruct*)(dest);
        //    return s;
        //}

        //template <class SomeStruct>
        //SomeStruct* deserialize(std::tuple<size_t, std::shared_ptr <char*> >& t) {
        //    auto len = std::get<0>(t);
        //    auto src = std::get<1>(t).get();
        //    auto dest = new char[std::get<0>(t)];
        //    memcpy(dest, src, len);
        //    SomeStruct* s = (SomeStruct*)(dest);
        //    return s;
        //}


        class EndPointException : public std::exception {
        public:
            EndPointException(boost::system::error_code ec) : m_ec(ec) {}

            virtual const char *what() const throw() {
                std::stringstream ss;
                ss << "code:" << m_ec.value() << "message:" << m_ec.message() << std::endl;
                return ss.str().c_str();
            }

        private:
            boost::system::error_code m_ec;
        };


        class utils {
        public:
            static asio::ip::udp::endpoint GetUdpEndpoint(std::string host, unsigned short port) {
                boost::system::error_code ec;
                // checks whether argument contains a valid IPv4 or IPv6 address then
                // instantiates a corresponding v4 || v6
               auto ip_address = asio::ip::address::from_string(host, ec);
                if (ec.value() != 0) {
                    std::cout << ec.value() << std::endl;
                    throw new EndPointException(ec);
                }
                auto ep = asio::ip::udp::endpoint(ip_address, port);
                return ep;
            }


        };
    }
}



