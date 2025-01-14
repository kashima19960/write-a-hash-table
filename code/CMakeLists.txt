cmake_minimum_required(VERSION 3.10)
set(CMAKE_GENERATOR "MinGW Makefiles")
project(Hash_table)

file(GLOB SRC_FILE 
${PROJECT_SOURCE_DIR}/src/*.c
${PROJECT_SOURCE_DIR}/src/*.h
${PROJECT_SOURCE_DIR}/*.c
)
include_directories(./src)

add_executable(${PROJECT_NAME} ${SRC_FILE})