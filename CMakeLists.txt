cmake_minimum_required(VERSION 3.10)
project(TrafficSignRecognition)

include(dlib/dlib/cmake)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")
set(CMAKE_BUILD_TYPE Release)
set(DLIB_NO_GUI_SUPPORT OFF)

option(USE_AVX_INSTRUCTIONS  "Compile your program with AVX instructions" OFF)

IF(USE_AVX_INSTRUCTIONS)
    add_definitions(-mavx)
    add_definitions(-march=native)
ENDIF()

MACRO(add_source name)
    add_executable(${name} src/${name}.cpp)
    TARGET_LINK_LIBRARIES(${name} dlib )
ENDMACRO()

add_executable(TrafficSignRecognition src/main.cpp)
add_source(hog_detector)
add_source(train_object_detector)
add_source(detect)
add_source(view_hog)