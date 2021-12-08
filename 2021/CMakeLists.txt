cmake_minimum_required(VERSION 3.17)

project(advent_of_code_2021)

function(add_advent_of_code_project PROBLEM_NUM)
  add_executable(advent_of_code_2021_${PROBLEM_NUM}
                 ${PROBLEM_NUM}/${PROBLEM_NUM}.cpp)
  target_link_libraries(advent_of_code_2021_${PROBLEM_NUM} fmt::fmt range-v3::range-v3 ctre::ctre)
endfunction()

add_advent_of_code_project(1)
add_advent_of_code_project(2)
add_advent_of_code_project(3)
add_advent_of_code_project(4)
if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
add_advent_of_code_project(5)
endif()
add_advent_of_code_project(7)
