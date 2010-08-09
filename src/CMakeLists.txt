
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)
add_subdirectory(sounds)
add_subdirectory(misc)
add_subdirectory(auth)
add_subdirectory(wizards)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(fusic_SRCS fusic.cpp main.cpp)
set(fusic_RCS res.qrc)
qt4_automoc(${fusic_SRCS})
qt4_add_resources( fusic_compiled_RCS ${fusic_RCS})
add_executable(fusic ${fusic_SRCS} ${fusic_compiled_RCS})
target_link_libraries(fusic ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} fusic_wizard fusic_misc)
