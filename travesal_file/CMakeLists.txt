cmake_minimum_required(VERSION 3.10)
project(Travesal_File VERSION 0.1.0)

message("-- " ${PROJECT_NAME} " " ${CMAKE_CXX_COMPILER})

set(CMAKE_CXX_STANDARD 11) #c++ 11

add_executable(Travesal_File "")
#添加头文件目录，相当于把路径添加到环境变量中。
target_include_directories(Travesal_File PUBLIC  ./)

file(GLOB SRCS CONFIGURE_DEPENDS *.cpp)
file(GLOB HEAD CONFIGURE_DEPENDS *.h )
#C++的源文件指定为Private，是因为源文件只是在构建库文件是使用，头文件指定为Public是因为构建和编译时都会使用。
target_sources(Travesal_File PRIVATE ${SRCS} PUBLIC ${HEAD})
