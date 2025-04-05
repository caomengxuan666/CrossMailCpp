if(POLICY CMP0148)
    cmake_policy(SET CMP0148 NEW)
endif()

cmake_minimum_required(VERSION 3.12)
project(CrossMailCpp CXX)

include(CheckIncludeFileCXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
check_include_file_cxx(any HAS_ANY)
check_include_file_cxx(string_view HAS_STRING_VIEW)
check_include_file_cxx(coroutine HAS_COROUTINE)
if (NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
    # Do nothing
elseif (HAS_ANY AND HAS_STRING_VIEW AND HAS_COROUTINE)
    set(CMAKE_CXX_STANDARD 20)
elseif (HAS_ANY AND HAS_STRING_VIEW)
    set(CMAKE_CXX_STANDARD 17)
else ()
    set(CMAKE_CXX_STANDARD 14)
endif ()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
find_package(pybind11 REQUIRED)

# python可执行路径
set(Python3_EXECUTABLE /usr/bin/python3)

# 查找 Python 开发包
find_package(Python3 REQUIRED COMPONENTS Development)

# 添加 Python 头文件路径
include_directories(${Python3_INCLUDE_DIRS})
find_package(pybind11 REQUIRED)

# 拷贝python文件夹下的所有.py文件
file(GLOB PYTHON_FILES "CrossMailCpp/python/*.py")
file(COPY ${PYTHON_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# 拷贝cxx目录下的所有文件
file(GLOB CXX_FILES "CrossMailCpp/*.yaml")
file(COPY ${CXX_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

aux_source_directory(CrossMailCpp CXX_SRC)

add_executable(emailexample example/sendemail.cxx)

target_link_libraries(emailexample PRIVATE pybind11::embed ${Python3_LIBRARIES})