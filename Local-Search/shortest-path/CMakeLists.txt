cmake_minimum_required(VERSION 3.5.0)
project(shortest_path VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add_library(Shortest_path STATIC Shortest_path.cpp)
# add_library(readData STATIC ../readData.cpp)

add_executable(main main.cpp Shortest_path.cpp ../readData.cpp)
add_executable(test_maker sparse_test_maker.cpp ../readData.cpp)
target_include_directories(main PRIVATE include)
target_include_directories(test_maker PRIVATE include)


set(CMAKE_VERBOSE_MAKEFILE ON)

# add_executable(shortest_path main.cpp)

# target_link_libraries(shortest_path readData)

# Link the test_maker executable with the library
#target_link_libraries(test_maker PRIVATE my_library)

#set(CMAKE_VERBOSE_MAKEFILEa OFF)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

include(CPack)


