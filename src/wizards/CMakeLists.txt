cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)
include_directories(${QT_INCLUDES}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${fusic_SOURCE_DIR}/src/auth/)

set(wizard_GUI_SRCS initialwizard_authsetup.ui)

set(wizard_SRCS databasechooser.cpp
                initialwizard_mysqlsetup.cpp
                initialwizard.cpp
                initialwizard_introduction.cpp
                initialwizard_authsetup.cpp)

qt4_wrap_ui(wizard_GUI_SRCS_H ${wizard_GUI_SRCS})
qt4_automoc(${wizard_SRCS})
add_library(fusic_wizard SHARED ${wizard_SRCS} ${wizard_GUI_SRCS_H})
target_link_libraries(fusic_wizard ${QT_QTCORE_LIBRARY} ${QT_QTSQL_LIBRARY})

