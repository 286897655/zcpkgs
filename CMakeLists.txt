cmake_minimum_required(VERSION 3.13.0)

project(zcpkgs)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# |---------------------zcpkgs-------------------------|

# |--------zpkg(zlog)----------|----zav----|----zio----|

# |---nlohmann json---spdlog---|---zpkg----|----zpkg---|

include (cmake/arch.cmake)
include (cmake/buildtype.cmake)
include (cmake/imports.cmake)

if(BUILD_RELEASE)
    message(STATUS "Build RELEASE VERSION")
else()
    message(STATUS "Build DEBUG VERSION")
endif()


#使能c++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # C++ 11 是强制要求，不会衰退至低版本
##set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
# 编译输出详细信息
set(CMAKE_VERBOSE_MAKEFILE ON)

#设置bin和lib库目录
set(BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin/debug/)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/debug/)
if(BUILD_RELEASE)
    set(BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin/release/)
    set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/release/)
endif()
message(STATUS "BIN_DIR:${BIN_DIR},LIB_DIR:${LIB_DIR}")
SET(EXECUTABLE_OUTPUT_PATH ${BIN_DIR})
SET(LIBRARY_OUTPUT_PATH ${LIB_DIR})

set(ZCPKGS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(APPEND_TARGET_INCLUDE)
set(APPEND_TARGET_LINK)
set(APPEND_TARGET_LIB)

# for assert.h(replace all assert to exception and don't abort)
include_directories(${ZCPKGS_ROOT}/capi/override/)
include_directories(${ZCPKGS_ROOT}/capi/)

# library
# zpkg(include zlog) some cc utility module for dev
# zav some av utility module for media frame work dev
# zio some io utility module for network dev

# |---------------------slivegateway/slivestreamproxy/slivesdkstream-------------------------------------|

add_subdirectory(pkgs/zpkg)
add_subdirectory(pkgs/zav)
add_subdirectory(pkgs/zio)

add_subdirectory(tests)