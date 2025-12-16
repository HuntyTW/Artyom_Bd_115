target_sources(cpp_compilation_lib
	PRIVATE
		mergesort.cpp
		mergevstav.cpp
	PUBLIC
		mergesort.hpp
		mergevstav.hpp
)

target_include_directories(cpp_compilation_lib PUBLIC
	"${PROJECT_SOURCE_DIR}/src"
)