# 查找在当前路径下的所有源文件并存储在变量SRCLIST中
aux_source_directory(./ SRCLIST)
# 添加编译器用于查找头文件的文件夹include
include_directories(../include)
# 从SRCLIST和example/main.c中生成名为工程名的可执行文件
add_executable(${PROJECT_NAME} ${SRCLIST} ${PROJECT_SOURCE_DIR}/example/main.c)
# 设置可执行文件输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
