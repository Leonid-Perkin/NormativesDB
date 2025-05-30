cmake_minimum_required(VERSION 3.10)
project(NormativesDB)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(NormativesDB
        src/main.cpp
        src/student.cpp
        src/validation.cpp
        src/file_operations.cpp
        src/student_manager.cpp
        src/txt_importer.cpp
)