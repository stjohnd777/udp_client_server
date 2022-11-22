project(upd_server)

add_executable(
        ${PROJECT_NAME}
        udp_server.cpp
        )

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        net
        )

target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
        net
        )
