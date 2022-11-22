project(test)

set(CMAKE_CXX_STANDARD 17)

add_executable(ServerTest catch.hpp ServerTest.cpp)
add_executable(ClientTest catch.hpp ClientTest.cpp)
add_executable(SerializerTest catch.hpp SerializerTest.cpp)

target_include_directories(ServerTest PUBLIC net)
target_include_directories(ClientTest PUBLIC net)
target_include_directories(SerializerTest PUBLIC net)

target_link_libraries(ServerTest PRIVATE net )
target_link_libraries(ClientTest PRIVATE net )
target_link_libraries(SerializerTest  PRIVATE net )