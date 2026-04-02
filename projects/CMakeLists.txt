cmake_minimum_required(VERSION 3.2...3.27.5)

project(Artyom_Bd_115)

enable_testing()
add_subdirectory(lib/googletest)

add_subdirectory(26_03_14_vector_list)

set(CMAKE_SUPPRESS_REGENERATION true)