cmake_minimum_required(VERSION 3.6)
project(PixelForge)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_SRC_DIR "${CMAKE_SOURCE_DIR}/src")

add_executable(
    ${PROJECT_NAME}

    ${PROJECT_SRC_DIR}/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)