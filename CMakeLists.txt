cmake_minimum_required(VERSION 2.8)
project(Remi)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmakeMods)
FIND_PACKAGE(OpenCV REQUIRED)
FIND_PACKAGE(Boost COMPONENTS system filesystem REQUIRED)




set(SOURCE_FILES
        main.cpp
        selector/Selector.cpp selector/Selector.h functionality.h)


link_directories(${Boost_LIBRARY_DIRS})

include_directories(
        ${Boost_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
)



add_executable(Remi ${SOURCE_FILES})
TARGET_LINK_LIBRARIES( Remi ${OpenCV_LIBS} ${Boost_LIBRARIES})

string(REPLACE "\\" "/" BOOSTDIR $ENV{BOOST_DIR})
string(REPLACE "\\" "/" OPENCVDIR $ENV{OPENCV_DIR})