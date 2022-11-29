#define CATCH_CONFIG_MAIN // provides main(); this line is required in only one .cpp file

#include "catch.hpp

#include "net.h"

#include <iostream>
#include <string>
#include <thread>
#include <memory>
 

using namespace std;
 


 TEST_CASE("RestClient2", "GetOneUUIDSync") {

	auto UUID = RestClient2::Get("http://127.0.0.1:8004/uuid");
	EXPECT_TRUE(true);
	std::cout << UUID << std::endl;
 

}


 TEST_CASE("RestClient2", "GetOneUUIDAsync" ) {
 
	 bool isDone = false;
	 string uri = "http://localhost:8004/uuid";
	 std::shared_ptr<std::thread>  t = RestClient2::Get(uri, [&](string res) {
		 cout << res << endl;
		 isDone = true;
		 }
	 );

	 t->join();
	 REQUIRE(isDone == true);
 }

 