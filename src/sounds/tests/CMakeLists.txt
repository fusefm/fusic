cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)
enable_testing()
include_directories(${QT_INCLUDES} 
		    ${CMAKE_CURRENT_BINARY_DIR} 
		    ${fusic_SOURCE_DIR}/src/sounds
		    ${fusic_SOURCE_DIR}/src/misc)

set(soundTest_SRCS soundtest.cpp)
qt4_automoc(${soundTest_SRCS})
add_executable(soundTest ${soundTest_SRCS})
add_test( test-sound soundTest )
target_link_libraries(soundTest ${QT_QTCORE_LIBRARY} ${QT_QTTEST_LIBRARY} fusic_sounds fusic_misc)
