project(test)

set(CMAKE_CXX_STANDARD 17)

add_executable(ServerTest catch.hpp ServerTest.cpp)
add_executable(ClientTest catch.hpp ClientTest.cpp)

target_link_libraries(ServerTest PRIVATE net )
target_link_libraries(ClientTest PRIVATE net )
