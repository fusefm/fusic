cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)
find_package(OpenLDAP REQUIRED)
include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(auth_SRCS authldap.cpp authbase.cpp)
qt4_automoc(${auth_SRCS})
add_library(fusic_auth SHARED ${auth_SRCS})
target_link_libraries(fusic_auth ${QT_QTCORE_LIBRARY})
