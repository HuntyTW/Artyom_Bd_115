target_sources(${CONTAINERS_LIB} 
	PRIVATE
		long_number.cpp
	PUBLIC
		long_number.hpp
)

target_include_directories(${CONTAINERS_LIB} PUBLIC
	${CURRENT_SOURCE_DIR}
)
