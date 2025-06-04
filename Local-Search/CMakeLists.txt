cmake_minimum_required(VERSION 3.12)
project(Local_Search VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

#include(CTest)
#enable_testing()

add_library(algorithm STATIC Algorithm.cpp)
add_library( path STATIC path.cpp)
add_library(readData STATIC readData.cpp)
add_library(randomize STATIC Randomize.cpp)
add_library(multiple-vehicle-bpmp STATIC multiple-vehicle-bpmp.cpp)



set(CMAKE_VERBOSE_MAKEFILE ON)
add_executable(Local_Search main.cpp)
add_executable(test_maker sparse_test_maker.cpp)
add_executable(randomize_main randomize_main.cpp)
add_executable(multiple-vehicle-bpmp-main multiple-vehicle-bpmp.cpp)


target_link_libraries(Local_Search algorithm readData path algorithm)
target_link_libraries(randomize_main algorithm readData path algorithm randomize)
target_link_libraries(test_maker readData )
target_link_libraries(multiple-vehicle-bpmp-main readData path algorithm )

# Link the test_maker executable with the library
#target_link_libraries(test_maker PRIVATE my_library)

#set(CMAKE_VERBOSE_MAKEFILEa OFF)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)


