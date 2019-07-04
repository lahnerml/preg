cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(preg)

# setup environment
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
    "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif (NOT CMAKE_BUILD_TYPE)

set (GOOD_FLAGS "-Wall -Wextra -pedantic -std=c++11")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GOOD_FLAGS}")

# Qt Includes
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

# define sources
INCLUDE_DIRECTORIES(${INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR}/src)
set (SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/mainwindow.cpp
  )

add_executable(preg ${SOURCES})
qt5_use_modules(preg Core Gui Widgets)
target_link_libraries(preg ${LIBRARIES})

add_custom_command(TARGET preg POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   ${CMAKE_CURRENT_SOURCE_DIR}/src/systemTrayIcon.png
                       $<TARGET_FILE_DIR:preg>
                   )
