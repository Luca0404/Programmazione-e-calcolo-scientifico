cmake_minimum_required(VERSION 3.10)

project(Esercitazione5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(avvio 
    codice_esercitazione_5.cpp 
    timecounter.cpp 
    timecounter.h 
    randfiller.h
)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")