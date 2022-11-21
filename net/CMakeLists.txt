cmake_minimum_required(VERSION 3.23)

project(net)

find_package(boost 1.74.0 REQUIRED)

add_library(${PROJECT_NAME}
        src/tcp/tcp_utils.cpp
        src/udp/udp_utils.cpp
        src/udp/udp_server.cpp
        include/net.h)

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                $<INSTALL_INTERFACE:include>
                ${Boost_INCLUDE_DIRS}
        PRIVATE
        )

target_link_libraries(
        ${PROJECT_NAME}
        ${Boost_LIBRARIES}
        PRIVATE
        )