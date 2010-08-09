cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)
find_package(OpenLDAP)
include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${fusic_SOURCE_DIR}/src/auth)

set(auth_SRCS authbase.cpp authsql.cpp authfactory.cpp)

IF(OpenLDAP_FOUND)
  add_definitions(-DLDAP_AUTH)
  set(auth_SRCS ${auth_SRCS} authldap.cpp)
ENDIF(OpenLDAP_FOUND)

qt4_automoc(${auth_SRCS})
add_library(fusic_auth SHARED ${auth_SRCS})
target_link_libraries(fusic_auth
                      ${QT_QTSQL_LIBRARY}
                      ${QT_QTCORE_LIBRARY}
                      ${OpenLDAP_LIBRARY})
