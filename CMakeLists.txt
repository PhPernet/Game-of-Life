cmake_minimum_required(VERSION 3.12)
project(GameOfLife LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*.cpp")

add_executable(GameOfLife ${SOURCES})

target_include_directories(GameOfLife PRIVATE include)

find_package(SFML 3.0 COMPONENTS Graphics Window System REQUIRED)

target_link_libraries(GameOfLife
    SFML::Graphics
    SFML::Window
    SFML::System
)
