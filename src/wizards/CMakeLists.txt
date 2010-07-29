cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)
include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(wizard_SRCS initialwizard.cpp initialwizard_introduction.cpp)
qt4_automoc(${wizard_SRCS})
add_library(fusic_wizard SHARED ${wizard_SRCS})
target_link_libraries(fusic_wizard ${QT_QTCORE_LIBRARY})
