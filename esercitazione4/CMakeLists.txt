cmake_minimum_required(VERSION 3.20)
project(esercitazione4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla")

#Per eseguire il programma, scrivo ./test_sorts nel terminale all'interno della cartella build (nome dato all'eseguibile)
add_executable(test_sorts 
    codice_esercitazione_4.cpp 
    timecounter.cpp
)

enable_testing()

add_test(NAME esecuzione_completa COMMAND test_sorts)
