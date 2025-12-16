cmake_minimum_required(VERSION 3.2...3.27.5)

project(Artyom_Bd_115)

enable_testing()
add_subdirectory(lib/googletest)

add_subdirectory(25_11_19_multi-level_menu)
add_subdirectory(25_09_20_cpp_compilation)

set(CMAKE_SUPPRESS_REGENERATION true)