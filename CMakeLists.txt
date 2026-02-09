cmake_minimum_required(VERSION 3.20)

project(
        STRSTeamsCheckIn
        VERSION 0.1.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(FETCHCONTENT_BASE_DIR
        ${CMAKE_SOURCE_DIR}/.cmake_cache
        CACHE PATH "FetchContent base directory"
)
include(FetchContent)

FetchContent_Declare(
        CLI11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
        GIT_TAG bfffd37e1f804ca4fae1caae106935791696b6a9
)
FetchContent_MakeAvailable(CLI11)

FetchContent_Declare(
        base64
        GIT_REPOSITORY https://github.com/tobiaslocker/base64.git
)
FetchContent_MakeAvailable(base64)

# Find the CURL package
find_package(CURL REQUIRED)

add_executable(
        STRSTeamsCheckIn
        src/main.cpp
        include/TeamsClient.h
        src/TeamsClient.cpp
        include/curl_wrappers/CurlHandle.h
        include/curl_wrappers/CurlHeaders.h
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
        CLI11
        base64
)