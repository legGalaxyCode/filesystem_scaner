cmake_minimum_required(VERSION 3.17)
project(file_analyzer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -gdwarf")

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(file_analyzer main.cpp hash_table.cpp hash_table.h hash_functions.cpp hash_functions.h)

#target_compile_options(file_analyzer ${MY_COMPILE_FLAGS})

target_link_libraries(file_analyzer -static-libstdc++)
target_link_libraries(file_analyzer ${Boost_LIBRARIES})
