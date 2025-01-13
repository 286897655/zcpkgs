#libzio

file(GLOB_RECURSE ZIO_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp 
                          ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/*.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/*.c
                          ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

add_library(zio STATIC ${ZIO_SRC})
target_include_directories(zio PUBLIC ${ZCPKGS_ROOT}/pkgs/)
target_link_libraries(zio PUBLIC zpkg pthread)