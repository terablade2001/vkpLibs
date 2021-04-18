cmake_minimum_required(VERSION 2.8.11)

set(PROJECTNAME "UpdateBuildVersion")
project(${PROJECTNAME})

set(CXX_FLAGS "-std=c++11 -o3 -Wall -Wno-unused-result")
set(C_FLAGS "-o3 -Wall -Wno-unused-result")

set(CMAKE_CONFIGURATION_TYPES "Release")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CXX_FLAGS}")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${C_FLAGS}")
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L. ")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")

file(GLOB CXX_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
)

add_executable(${PROJECTNAME} ${CXX_SRCS} ${C_SRCS})
target_link_libraries(${PROJECTNAME} ${LINK_OPTIONS})
