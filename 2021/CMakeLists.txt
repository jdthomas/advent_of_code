cmake_minimum_required(VERSION 3.17)

project(advent_of_code_2021)

# set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

function(add_advent_of_code_project PROBLEM_NUM)
  add_executable(advent_of_code_2021_${PROBLEM_NUM}
                 ${PROBLEM_NUM}/${PROBLEM_NUM}.cpp)
  target_link_libraries(advent_of_code_2021_${PROBLEM_NUM} fmt::fmt range-v3::range-v3)
endfunction()

set(CMAKE_C_FLAGS " -Wall -Wextra -g ")

find_package(fmt REQUIRED)
include_directories(${fmt_INCLUDE_DIRS})

find_package(range-v3 REQUIRED)
include_directories(${range-v3_INCLUDE_DIRS})

add_advent_of_code_project(1)
add_advent_of_code_project(2)
add_advent_of_code_project(3)
