cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(misc_SRCS fusicsettings.cpp)
qt4_automoc(${misc_SRCS})
add_library(fusic_misc SHARED ${misc_SRCS})
target_link_libraries(fusic_misc ${QT_QTCORE_LIBRARY})
