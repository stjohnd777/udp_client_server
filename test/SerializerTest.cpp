#define CATCH_CONFIG_MAIN // provides main(); this line is required in only one .cpp file
#include "catch.hpp"

#include <iostream>
#include <sstream>
#include <tuple>
#include <net.h>

using namespace std;

typedef struct example {
    uint8_t _u8;
    uint16_t _u16;
    uint32_t _u32;
    char matrix [100][200]; 
} Test;

TEST_CASE( "_Serializer Test ", "1" ) {
    REQUIRE( true == true );

    Test test;
    test._u8 = 1;
    test._u16 = 2;
    test._u32 = 3;

    //std::tuple<size_t, std::shared_ptr <char*> > t = lm::spp::serialize(test);
    std::tuple<size_t,  char* > t = lm::spp::_serialize(test);


    Test* test2 = lm::spp::_deserialize<Test>(t);

    REQUIRE(test._u8 ==  test2->_u8);
    REQUIRE(test._u16 == test2->_u16);
    REQUIRE(test._u32 == test2->_u32);
    
}


TEST_CASE("Serializer Test ", "2") {
    REQUIRE(true == true);

    Test test;
    test._u8 = 1;
    test._u16 = 2;
    test._u32 = 3;

    std::tuple<size_t, std::shared_ptr <char*> > t = lm::spp::serialize(test);
   

    Test* test2 = lm::spp::deserialize<Test>(t);

    REQUIRE(test._u8 == test2->_u8);
    REQUIRE(test._u16 == test2->_u16);
    REQUIRE(test._u32 == test2->_u32);

}