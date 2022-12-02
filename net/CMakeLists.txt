project(net)
set(CMAKE_CXX_STANDARD 17)

find_package(boost 1.74.0 REQUIRED)
find_package(CURL CONFIG REQUIRED)
 

 

add_library(
        ${PROJECT_NAME}
        include/udp/udp_utils.h
        include/Props.h
        src/Props.cpp
        include/TimeMetric.h
        src/TimeMetric.cpp
        include/RandomString.h
        src/RandomString.cpp

        # include/crow_all.h
        # include/http/RestClient.h
        # src/http/RestClient.cpp


        src/udp/udp_utils.cpp
        include/utils.h
        include/net.h
        )

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
        CURL::libcurl
        )
        