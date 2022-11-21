cmake_minimum_required(VERSION 3.23)

project(upd_server)

add_executable(
        ${PROJECT_NAME}
        udp_server.cpp
        )

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        udp_lib
        )

target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
        udp_lib
        )
