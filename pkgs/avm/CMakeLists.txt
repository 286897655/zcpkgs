#libavm

file(GLOB_RECURSE AVM_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/*.c
                          ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

add_library(avm STATIC ${AVM_SRC})
target_include_directories(avm PUBLIC ${ZCPKGS_ROOT}/pkgs/)
target_compile_options(avm PRIVATE -DG722_1_USE_FIXED_POINT)
target_link_libraries(avm PUBLIC zpkg)