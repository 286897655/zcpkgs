# libzpkg

set(ZPKG_SRC_ROOT ${ZCPKGS_ROOT}/pkgs/zpkg)

file(GLOB_RECURSE ZPKG_SRC  ${ZPKG_SRC_ROOT}/*.h
                            ${ZPKG_SRC_ROOT}/*.cc
                            ${ZPKG_SRC_ROOT}/*.hpp
                            ${ZPKG_SRC_ROOT}/*.cpp
                            ${ZPKG_SRC_ROOT}/*.c)
list(APPEND ZPKG_SRC ${ZCPKGS_ROOT}/pkgs/zlog/log.cc)

add_library(zpkg STATIC ${ZPKG_SRC})
target_include_directories(zpkg PRIVATE ${ZPKG_SRC_ROOT}/pkgs/zpkg/)
target_include_directories(zpkg PUBLIC ${ZCPKGS_ROOT}/include/)

if(ARCH_AMD64)
    target_compile_options(zpkg PUBLIC -mavx2)
endif()