cmake_minimum_required(VERSION 3.19)


project(advent_of_code_2020)

set(CMAKE_CXX_STANDARD 17)


add_executable(advent_of_code_2020_1 1/1.cpp)
add_executable(advent_of_code_2020_2 2/2.cpp)
add_executable(advent_of_code_2020_3 3/3.cpp)
add_executable(advent_of_code_2020_4 4/4.cpp)
add_executable(advent_of_code_2020_5 5/5.cpp)
add_executable(advent_of_code_2020_6 6/6.cpp)
add_executable(advent_of_code_2020_7 7/7.cpp)
add_executable(advent_of_code_2020_8 8/8.cpp)

set(CMAKE_C_FLAGS " -Wall -Wextra -g ")


find_package(fmt REQUIRED)
include_directories(${fmt_INCLUDE_DIRS})
target_link_libraries(advent_of_code_2020_1 fmt::fmt)
target_link_libraries(advent_of_code_2020_2 fmt::fmt)
target_link_libraries(advent_of_code_2020_3 fmt::fmt)
target_link_libraries(advent_of_code_2020_4 fmt::fmt)
target_link_libraries(advent_of_code_2020_5 fmt::fmt)
target_link_libraries(advent_of_code_2020_6 fmt::fmt)
target_link_libraries(advent_of_code_2020_7 fmt::fmt)
target_link_libraries(advent_of_code_2020_8 fmt::fmt)

find_package(range-v3 REQUIRED)
include_directories(${range-v3_INCLUDE_DIRS})
target_link_libraries(advent_of_code_2020_1 range-v3::range-v3)
target_link_libraries(advent_of_code_2020_2 range-v3::range-v3)
target_link_libraries(advent_of_code_2020_3 range-v3::range-v3)
target_link_libraries(advent_of_code_2020_4 range-v3::range-v3)
target_link_libraries(advent_of_code_2020_5 range-v3::range-v3)
target_link_libraries(advent_of_code_2020_6 range-v3::range-v3)
target_link_libraries(advent_of_code_2020_7 range-v3::range-v3)
target_link_libraries(advent_of_code_2020_8 range-v3::range-v3)
