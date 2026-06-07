cmake_minimum_required(VERSION 3.11)
project(PixelForge)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)
find_package(glfw3 REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

set(PROJECT_SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(PROJECT_INC_DIR "${CMAKE_SOURCE_DIR}/include")

add_executable(
    ${PROJECT_NAME}

    ${PROJECT_SRC_DIR}/main.cpp
    ${PROJECT_SRC_DIR}/system.cc
    ${PROJECT_SRC_DIR}/Display/video.cc
    ${PROJECT_SRC_DIR}/IO/keypad.cc
    ${PROJECT_SRC_DIR}/Memory/ram.cc
    ${PROJECT_SRC_DIR}/Processor/cpu.cc
    ${PROJECT_SRC_DIR}/Processor/stack.cc
    ${PROJECT_SRC_DIR}/Timer/delay.cc
    ${PROJECT_SRC_DIR}/Timer/sound.cc
    ${PROJECT_SRC_DIR}/Util/rom_loader.cc
    ${PROJECT_SRC_DIR}/Util/Window/manager.cc
    ${PROJECT_SRC_DIR}/Util/Window/glad.c
)

set_target_properties(PixelForge PROPERTIES MACOSX_BUNDLE TRUE)

target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRECTORIES})
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/Util/Window
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${GLFW_LIBRARIES} dl)

target_compile_options(${PROJECT_NAME} PRIVATE ${GLFW_CFLAGS_OTHER})