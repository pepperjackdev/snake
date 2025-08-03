cmake_minimum_required(VERSION 3.10)
project(snake VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/include)

# TODO: Should be avoided
file(GLOB SRC_FILES
    "src/*.c"
    "src/**/*.c"
)

add_executable(snake ${SRC_FILES})

find_package(PkgConfig REQUIRED)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET glib-2.0 raylib)
target_link_libraries(snake PkgConfig::deps)

add_custom_target(run
    COMMAND snake
    DEPENDS snake
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running MyProject..."
)
