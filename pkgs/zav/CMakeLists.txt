#libzav

file(GLOB_RECURSE ZAV_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp 
                          ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
                          ${CMAKE_CURRENT_SOURCE_DIR}/*.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/*.c
                          ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

add_library(zav STATIC ${ZAV_SRC})
target_include_directories(zav PUBLIC ${ZCPKGS_ROOT}/pkgs/)
target_compile_options(zav PRIVATE -DG722_1_USE_FIXED_POINT)
target_link_libraries(zav PRIVATE zpkg)




