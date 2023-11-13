# 限制cmake版本
cmake_minimum_required(VERSION 2.8)
# 设置工程名为NEWBMI，可通过预定义变量PROJECT_NAME读取
project(DEMO1)
# 如果没有确定CMAKE类型则设置为Release类型
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release)
ENDIF()
# 输出Build type
# MESSAGE("-- Build type: " ${CMAKE_BUILD_TYPE})
# 设置C和C++编译标志，' -Wall '标志启用所有警告，' -O3 '标志启用最高级别的优化，' -march=native '标志针对主机的体系结构优化代码
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall  -O3 -march=native ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall   -O3 -march=native")
# 检查C++编译器是否支持给定的标志
include(CheckCXXCompilerFlag)
# 使用标志-std=c++11编译程序，并将结果存储在变量COMPILER_SUPPORTS_CXX11中
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
   # 将编译器的C++标准设置为C++14
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
   # 添加宏定义 调试用
   add_definitions(-DCOMPILEDWITHC11)
   message(STATUS "Using flag -std=c++14.")
elseif(COMPILER_SUPPORTS_CXX0X)
   # 将编译器的C++标准设置为C++0x
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
   # 添加宏定义 调试用
   add_definitions(-DCOMPILEDWITHC0X)
   message(STATUS "Using flag -std=c++0x.")
else()
   message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
endif()
# 加入cmake_modules的位置
LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake_modules)
MESSAGE("-- Set up project-related libraries")
############################################################################################################################
# “QUIET”选项会抑制“find_package”命令的输出
find_package(OpenCV 4.2 QUIET)
if(NOT OpenCV_FOUND)
   find_package(OpenCV 2.4.3 QUIET)
   if(NOT OpenCV_FOUND)
      message(FATAL_ERROR "OpenCV > 2.4.3 not found.")
   endif()
else()
   MESSAGE("-- opencv version is : 4.2.0")
endif()
# NO_MODULE选项指定FindEigen3.cmake 
find_package(Eigen3 3.1.0 REQUIRED NO_MODULE)
#添加Pangolin依赖
find_package(Pangolin REQUIRED)
include_directories(
${PROJECT_SOURCE_DIR}
${PROJECT_SOURCE_DIR}/include
${EIGEN3_INCLUDE_DIR}
${Pangolin_INCLUDE_DIRS}
)
# 指定应放置编译库文件的目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
# 添加动态库
add_library(${PROJECT_NAME} SHARED
src/Camera_pose.cpp
src/slam_visualizer.cpp
)
# 链接指定库
target_link_libraries(${PROJECT_NAME}
${OpenCV_LIBS}
${EIGEN3_LIBS}
${Pangolin_LIBRARIES}
)
# 构建示例
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Examples/Dataset_DEMO)
add_executable(datasetDemo Examples/Dataset_DEMO/main.cpp)
target_link_libraries(datasetDemo ${PROJECT_NAME})

