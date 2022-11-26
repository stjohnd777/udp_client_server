project(net)

find_package(boost 1.74.0 REQUIRED)

add_library(${PROJECT_NAME}
        include/udp/udp_utils.h
        include/TimeMetric.h
        src/TimeMetric.cpp
        src/udp/udp_utils.cpp
        include/utils.h
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