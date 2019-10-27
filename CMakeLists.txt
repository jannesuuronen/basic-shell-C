cmake_minimum_required(VERSION 3.5)
project(basic-shell-C)
include_directories("${PROJECT_SOURCE_DIR}/include")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_subdirectory (shell)


add_executable(Basic-shell-C src/main.c)
target_link_libraries(Basic-shell-C shell)
