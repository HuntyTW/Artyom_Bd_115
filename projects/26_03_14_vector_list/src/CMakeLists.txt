target_sources(${CONTAINERS_LIB} 
	PRIVATE
		doubly_linked_list.cpp
		vector.cpp
		templates_doubly_linked_list.cpp
		templates_vector.cpp
	PUBLIC
		doubly_linked_list.hpp
		vector.hpp
)

target_include_directories(${CONTAINERS_LIB} PUBLIC
	${CURRENT_SOURCE_DIR}
)
