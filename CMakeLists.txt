cmake_minimum_required(VERSION 3.10)
project(my_project VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/include)

# TODO: Should be avoided
file(GLOB SRC_FILES
    "src/*.c"
    "src/**/*.c"
)

find_package(raylib CONFIG REQUIRED)

add_executable(my_project ${SRC_FILES})

target_link_libraries(my_project PRIVATE raylib)

add_custom_target(run
    COMMAND my_project
    DEPENDS my_project
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running MyProject..."
)