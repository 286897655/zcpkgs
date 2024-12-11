#libzio

file(GLOB_RECURSE ZIO_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp 
                          ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/*.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/*.c
                          ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

add_library(zio STATIC ${ZIO_SRC})
target_include_directories(zio PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(zio PUBLIC ${ZCPKGS_ROOT}/include)
target_link_libraries(zio PUBLIC zpkg pthread)