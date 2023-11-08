# 限制cmake版本
cmake_minimum_required(VERSION 2.8)
# 设置工程名为NEWBMI，可通过预定义变量PROJECT_NAME读取
project(NEWBMI)
# 添加一个子目录并构建该子目录，若找不到目录即退出cmake
add_subdirectory(./src)

