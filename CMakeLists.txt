cmake_minimum_required(VERSION 3.11)
project(
    PixelForge
    VERSION 1.0.0
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(glfw3 3.4 QUIET)

if(glfw3_FOUND)
    message(STATUS "Found system GLFW. Bypassing network fetch.")
else()
    message(STATUS "System GLFW not found. Fetching from source for CI/CD...")
    
    include(FetchContent)
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.4
        GIT_SHALLOW    ON # Enforce shallow clone to reduce payload size
    )

    # Disable sub-builds to speed up CI/CD pipeline times
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(glfw)
endif()

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

if(APPLE)
    set_target_properties(PixelForge PROPERTIES MACOSX_BUNDLE TRUE)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRECTORIES})
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include

    # Included for valid path to OpenGL helpers (glad, KHR)
    ${CMAKE_SOURCE_DIR}/include/Util/Window
)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

if (UNIX AND NOT APPLE)
    target_link_libraries(PixelForge PRIVATE m dl)
endif()