cmake_minimum_required(VERSION 3.10)
project(snake VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

# Include path
include_directories(${CMAKE_SOURCE_DIR}/include)

# Collect all .c source files recursively
file(GLOB_RECURSE SRC_FILES
    "src/*.c"
    "src/**/*.c"
)

# Create the executable
add_executable(snake ${SRC_FILES})

# Platform-specific configuration
if(EMSCRIPTEN)
    message(STATUS "Configuring for Emscripten/WebAssembly build")

    set(RAYLIB_DIR "C:/Dev/sources/raylib")
    set(RAYLIB_BUILD_DIR "${RAYLIB_DIR}/build")

    target_include_directories(snake PRIVATE
        "${RAYLIB_DIR}/src"
        
    )

    target_link_libraries(snake PRIVATE
        "${CMAKE_SOURCE_DIR}/lib/wasm/raylib.a"
        "-s USE_GLFW=3"
        "-s FULL_ES2=1"
        "-s ALLOW_MEMORY_GROWTH=1"
        "-s ASYNCIFY"
        "-s EXPORTED_RUNTIME_METHODS=['requestFullscreen']"
        "--shell-file ${CMAKE_SOURCE_DIR}/shell.html"
    )

    set_target_properties(snake PROPERTIES SUFFIX ".html")

    target_compile_options(snake PRIVATE -O2) 

else()
    message(STATUS "Configuring for native build")

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(deps REQUIRED IMPORTED_TARGET raylib)

    target_include_directories(snake PRIVATE ${deps_INCLUDE_DIRS})
    target_link_libraries(snake PRIVATE PkgConfig::deps)
    target_compile_options(snake PRIVATE -O2)
endif()

# Optional run target
add_custom_target(run
    COMMAND snake
    DEPENDS snake
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running snake..."
)
