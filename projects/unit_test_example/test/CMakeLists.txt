set(SOURCES
	test_cpp_compilation.cpp
)

add_executable(test_cpp_compilation ${SOURCES})
target_link_libraries(test_cpp_compilation
	gtest_main
	gmock_main
	cpp_compilation_lib
)

add_test(test_cpp_compilation test_cpp_compilation)