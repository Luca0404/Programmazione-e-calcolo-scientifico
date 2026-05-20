cmake_minimum_required(VERSION 3.12)
project(Esercitazione10 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Eigen3 REQUIRED)

add_executable(avvio codice_esercitazione_10.cpp)

target_link_libraries(avvio PRIVATE Eigen3::Eigen)