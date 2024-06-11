cmake_minimum_required(VERSION 3.10)
project(SAR-GL-Remake VERSION 0.1.0 LANGUAGES CXX C)

set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/out")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/${CMAKE_BUILD_TYPE}")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(spdlog REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/dependencies/include
)

link_libraries(
    ${OPENGL_LIBRARIES}
    spdlog::spdlog
    glfw3
)

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/dependencies/src/*.cpp
    ${CMAKE_SOURCE_DIR}/dependencies/src/*.c
)

add_executable(SAR ${SRC_FILES})

file(GLOB_RECURSE ASSETS_FILES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/assets/*
)

foreach(ASSET_FILE ${ASSETS_FILES})
    file(COPY ${ASSET_FILE} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)
endforeach()
