project(udp_client)

add_executable(
        udp_client
        udp_client.cpp
        )

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        net
        )

target_link_libraries(${PROJECT_NAME}
        PUBLIC
        net
        )
