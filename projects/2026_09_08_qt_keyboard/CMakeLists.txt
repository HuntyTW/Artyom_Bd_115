cmake_minimum_required(VERSION 3.2...3.27.5)

project(keyboard VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        src/main.cpp
		src/keyboard.cpp
		src/keyboard_button.cpp
		src/keyboard_data.cpp
		src/keyboard_window.cpp
		src/welcome_window.cpp
)

qt_add_executable(${PROJECT_NAME}
	MANUAL_FINALIZATION
	${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)

qt_finalize_executable(${PROJECT_NAME})

# Автотесты на QTest (по умолчанию не собираются):
#   cmake -DKEYBOARD_BUILD_TESTS=ON ..
option(KEYBOARD_BUILD_TESTS "Build QTest tests" OFF)
if(KEYBOARD_BUILD_TESTS)
	find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

	set(TEST_SOURCES ${PROJECT_SOURCES})
	list(REMOVE_ITEM TEST_SOURCES src/main.cpp)

	add_executable(keyboard_test tests/test_keyboard.cpp ${TEST_SOURCES})
	target_include_directories(keyboard_test PRIVATE src)
	target_link_libraries(keyboard_test PRIVATE
		Qt${QT_VERSION_MAJOR}::Widgets
		Qt${QT_VERSION_MAJOR}::Test
	)
endif()
