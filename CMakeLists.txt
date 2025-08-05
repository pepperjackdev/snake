cmake_minimum_required(VERSION 3.10)
project(snake VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Collect source files
file(GLOB_RECURSE SRC_FILES
    "src/*.c"
    "src/**/*.c"
)

add_executable(snake ${SRC_FILES})

if(EMSCRIPTEN)
    message(STATUS "Configuring for Emscripten/WebAssembly build")

    # Include raylib headers
    target_include_directories(snake PRIVATE
        "C:/Dev/sources/raylib/src"
    )

    # Link the static library built for wasm
    target_link_libraries(snake PRIVATE
        "C:/Dev/sources/raylib/build/raylib/libraylib.a"
        "-s USE_GLFW=3"
        "-s FULL_ES2=1"
        "-s ALLOW_MEMORY_GROWTH=1"
    )

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -sASYNCIFY")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -sASYNCIFY")


    set_target_properties(snake PROPERTIES SUFFIX ".html")

    target_compile_options(snake PRIVATE -O2)
else()
    # Native build path

    message(STATUS "Configuring for native build")

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(deps REQUIRED IMPORTED_TARGET glib-2.0 raylib)

    target_link_libraries(snake PRIVATE PkgConfig::deps)

    # Optional: native compile flags (e.g., optimization)
    target_compile_options(snake PRIVATE -O2)

endif()

add_custom_target(run
    COMMAND snake
    DEPENDS snake
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running MyProject..."
)
