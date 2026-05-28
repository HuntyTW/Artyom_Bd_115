add_executable(test_long_number test_long_number.cpp) 

target_link_libraries(test_long_number 
    gtest 
    ${CONTAINERS_LIB}
)

target_include_directories(test_long_number PRIVATE ../src)

add_test(test_long_number test_long_number)