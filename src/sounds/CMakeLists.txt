

cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR} ${fusic_SOURCE_DIR}/src/misc)

set(sound_SRCS song.cpp sound.cpp)
qt4_automoc(${sound_SRCS})
add_library(fusic_sounds SHARED ${sound_SRCS})
target_link_libraries(fusic_sounds ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
