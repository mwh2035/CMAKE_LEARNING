# PROJECT指定工程名称，也可指定编程语言
PROJECT (HELLO)
# 显示定义变量 可以书写多个.c文件
SET(SRC_LIST main.c)
# 向终端输出用户定义信息
MESSAGE(STATUS "This is BINARY dir " ${HELLO_BINARY_DIR})
MESSAGE(STATUS "This is SOURCE dir "${HELLO_SOURCE_DIR})
# 从显示定义变量SRC_LIST 生成名为hello的可执行文件 ${}为变量应用格式
ADD_EXECUTABLE(BMI ${SRC_LIST})
