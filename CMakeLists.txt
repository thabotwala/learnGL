cmake_minimum_required(VERSION 3.10)
project(learnopengl)

#set c++ version
set(CXX_MINIMUM_STANDARD 11)
set(CXX_MINIMUM_STANDARD_REQUUIRED ON)

#set cxx flags
#The -03 is for the compiler to optimize for speed
set(CMAKE_CXX_FLAGS, "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -03")

#set cmake include directories
include_directories("${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/include")
#add src directories
add_subdirectory(${PROJECT_SOURCE_DIR}/src)
#set cmake sources
set(SOURCES 
  main.cpp
  )
add_executable(${PROJECT_NAME} ${SOURCES})
