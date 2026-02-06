# CMakeLists.txt
cmake_minimum_required(VERSION 3.20)

project(
        STRSTeamsCheckIn
        VERSION 0.1.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find the CURL package
find_package(CURL REQUIRED)

add_executable(
        STRSTeamsCheckIn
        src/main.cpp
        include/TeamsClient.h
        src/TeamsClient.cpp
)

target_include_directories(
        STRSTeamsCheckIn
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

# Link CURL library
target_link_libraries(
        STRSTeamsCheckIn
        PRIVATE
        CURL::libcurl
)