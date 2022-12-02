#define CATCH_CONFIG_MAIN // provides main(); this line is required in only one .cpp file
#include "catch.hpp"

#include <iostream>
#include <sstream>
#include <tuple>
#include <net.h>

using namespace std;

//typedef struct example {
//    uint8_t _u8;
//    uint16_t _u16;
//    uint32_t _u32;
//    char matrix [100][200]; 
//} Test;


TEST_CASE("Serializer Test ", "Simple Small struct") {
    Test in;
    in._u8 = 1;
    in._u16 = 2;
    in._u32 = 3;

    char*   t = lm::spp::Serialize(in);
    Test* out = lm::spp::DeSerialize<Test>(t);

    REQUIRE(in._u8 == out->_u8);
    REQUIRE(in._u16 == out->_u16);
    REQUIRE(in._u32 == out->_u32);
}


