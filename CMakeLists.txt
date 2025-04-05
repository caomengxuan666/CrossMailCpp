if(POLICY CMP0148)
    cmake_policy(SET CMP0148 NEW)
endif()

cmake_minimum_required(VERSION 3.12)
project(CrossMailCpp CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(CheckIncludeFileCXX)
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

# 查找 pybind11 和 Python 开发包
find_package(pybind11 REQUIRED)
set(Python3_EXECUTABLE /usr/bin/python3)
find_package(Python3 REQUIRED COMPONENTS Development)

# 拷贝 Python 文件
file(GLOB PYTHON_FILES "CrossMailCpp/python/*.py")
file(COPY ${PYTHON_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# 安装 Python 文件
install(FILES ${PYTHON_FILES}
    DESTINATION lib/python
)

# 拷贝配置文件
file(GLOB CXX_FILES "CrossMailCpp/*.yaml")
file(COPY ${CXX_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# 安装配置文件
install(FILES ${CXX_FILES}
    DESTINATION lib/config
)

# 拷贝头文件到构建目录
file(COPY CrossMailCpp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# 定义 CrossMailCpp 接口库
add_library(CrossMailCpp INTERFACE)

# 添加头文件路径
target_include_directories(CrossMailCpp INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/CrossMailCpp>
    $<INSTALL_INTERFACE:include>
)

# 链接依赖项
target_link_libraries(CrossMailCpp INTERFACE
    pybind11::embed                          # pybind11 嵌入式解释器
    ${Python3_LIBRARIES}                     # Python 库
)

# 安装接口库的目标
install(TARGETS CrossMailCpp EXPORT CrossMailCppTargets)

# 安装头文件
install(DIRECTORY CrossMailCpp
    DESTINATION include
)

# 导出配置文件
install(EXPORT CrossMailCppTargets
    FILE CrossMailCppConfig.cmake
    NAMESPACE CrossMailCpp::
    DESTINATION lib/cmake/CrossMailCpp
)

# 示例程序
add_executable(emailexample example/sendemail.cxx)

# 链接 CrossMailCpp 接口库
target_link_libraries(emailexample PRIVATE CrossMailCpp)

# 安装示例程序
install(TARGETS emailexample
    RUNTIME DESTINATION bin
)