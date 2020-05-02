cmake_minimum_required(VERSION 3.16)
project(SADAS_GDB)

set(APP_DOMAIN ./src)

#Include the directories and now your cpp files will recognize your headers
include_directories(${APP_DOMAIN})

set(CMAKE_CXX_STANDARD 17)


add_executable(SADAS_GDB main.cpp src/C++/CTP_graph.cpp src/C++/CTP_graph.h src/C++/NEXI_graph.cpp src/C++/NEXI_graph.h src/C++/prova.cpp src/C++/test_blue.cpp src/C++/utils.h src/C++/test_orange.cpp)