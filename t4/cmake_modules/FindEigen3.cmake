# - 寻找Eigen3库
#
# 本模块可以设置Eigen3最小要求版本
# 如果没设置Eigen3_FIND_VERSION
if(NOT Eigen3_FIND_VERSION)
  if(NOT Eigen3_FIND_VERSION_MAJOR)
    set(Eigen3_FIND_VERSION_MAJOR 2)
  endif(NOT Eigen3_FIND_VERSION_MAJOR)
  if(NOT Eigen3_FIND_VERSION_MINOR)
    set(Eigen3_FIND_VERSION_MINOR 91)
  endif(NOT Eigen3_FIND_VERSION_MINOR)
  if(NOT Eigen3_FIND_VERSION_PATCH)
    set(Eigen3_FIND_VERSION_PATCH 0)
  endif(NOT Eigen3_FIND_VERSION_PATCH)
  # 将主、次和补丁版本号分别设置为2,91和0
  # 保存到Eigen3_FIND_VERSION中
  set(Eigen3_FIND_VERSION "${Eigen3_FIND_VERSION_MAJOR}.${Eigen3_FIND_VERSION_MINOR}.${Eigen3_FIND_VERSION_PATCH}")
endif(NOT Eigen3_FIND_VERSION)

# CMAKE宏 ,在主cmake文件中以读取Eigen情况下才会调用宏
macro(_eigen3_check_version)
  # 读取位于${eigen3_include_dir}/eigen/src/core/util/中的macros.h文件 并存入_eigen3_version_header
  file(READ "${EIGEN3_INCLUDE_DIR}/Eigen/src/Core/util/Macros.h" _eigen3_version_header)
  # 从macros.h匹配 define EIGEN_WORLD_VERSION [0-9] ,获取数字存入_eigen3_world_version_match
  string(REGEX MATCH "define[ \t]+EIGEN_WORLD_VERSION[ \t]+([0-9]+)" _eigen3_world_version_match "${_eigen3_version_header}")
  set(EIGEN3_WORLD_VERSION "${CMAKE_MATCH_1}")
  string(REGEX MATCH "define[ \t]+EIGEN_MAJOR_VERSION[ \t]+([0-9]+)" _eigen3_major_version_match "${_eigen3_version_header}")
  set(EIGEN3_MAJOR_VERSION "${CMAKE_MATCH_1}")
  string(REGEX MATCH "define[ \t]+EIGEN_MINOR_VERSION[ \t]+([0-9]+)" _eigen3_minor_version_match "${_eigen3_version_header}")
  set(EIGEN3_MINOR_VERSION "${CMAKE_MATCH_1}")
  # 设置从macros.h读取到的版本号存入EIGEN3_VERSION
  set(EIGEN3_VERSION ${EIGEN3_WORLD_VERSION}.${EIGEN3_MAJOR_VERSION}.${EIGEN3_MINOR_VERSION})
  # 读取的EIGEN3_VERSION 小于 设置的Eigen3_FIND_VERSION
  if(${EIGEN3_VERSION} VERSION_LESS ${Eigen3_FIND_VERSION})
    set(EIGEN3_VERSION_OK FALSE)
  else(${EIGEN3_VERSION} VERSION_LESS ${Eigen3_FIND_VERSION})
    set(EIGEN3_VERSION_OK TRUE)
  endif(${EIGEN3_VERSION} VERSION_LESS ${Eigen3_FIND_VERSION})
  # 输出EIGEN3_VERSION_OK 区分版本大于设置版本还是小于设置版本
  
  # 如果读取版本小于设置版本
  if(NOT EIGEN3_VERSION_OK)
    message(STATUS "Eigen3 version ${EIGEN3_VERSION} found in ${EIGEN3_INCLUDE_DIR}, "
                   "but at least version ${Eigen3_FIND_VERSION} is required")
  endif(NOT EIGEN3_VERSION_OK)
endmacro(_eigen3_check_version)

# 如果设置了 EIGEN3_INCLUDE_DIR
if (EIGEN3_INCLUDE_DIR)
  # makefile cache文件中已经读取
  # 通过宏读取版本号
  _eigen3_check_version()
  set(EIGEN3_FOUND ${EIGEN3_VERSION_OK})

else (EIGEN3_INCLUDE_DIR)

  # 判断是否为win32
  if (WIN32)
    set(EIGEN_ADDITIONAL_SEARCH_PATHS ${EIGEN_ADDITIONAL_SEARCH_PATHS} "C:/Program Files/Eigen/include" "C:/Program Files (x86)/Eigen/include")
  endif(WIN32)
  # 寻找signature_of_eigen3_matrix_library文件 从${CMAKE_INSTALL_PREFIX}/include(CMAKE_INSTALL_PREFIX默认为usr/local),${EIGEN_ADDITIONAL_SEARCH_PATHS},${KDE4_INCLUDE_DIR}
  find_path(EIGEN3_INCLUDE_DIR NAMES signature_of_eigen3_matrix_library
      PATHS
      ${CMAKE_INSTALL_PREFIX}/include
      ${EIGEN_ADDITIONAL_SEARCH_PATHS}
      ${KDE4_INCLUDE_DIR}
      PATH_SUFFIXES eigen3 eigen
    )
   # PATH_SUFFIXES：在每个搜索路径的子目录下搜索eigen3 eigen
  # 读取完成 利用宏检索Eigen版本
  if(EIGEN3_INCLUDE_DIR)
    _eigen3_check_version()
  endif(EIGEN3_INCLUDE_DIR)
  
  include(FindPackageHandleStandardArgs)
  # FindPackageHandleStandardArgs.cmake 是 CMake 内置的一个模块，它用于定义一个宏，可以用于在找到一个包之后，对其进行一些标准化的处理,它通常用于在CMake中查找和配置外部库或工具包
  find_package_handle_standard_args(Eigen3 DEFAULT_MSG EIGEN3_INCLUDE_DIR EIGEN3_VERSION_OK)
  # 将命名的缓存变量标记为高级
  mark_as_advanced(EIGEN3_INCLUDE_DIR)

endif(EIGEN3_INCLUDE_DIR)

