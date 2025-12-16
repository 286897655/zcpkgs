#libiom
#io module
file(GLOB_RECURSE IOM_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.h
                          ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

add_library(iom STATIC ${IOM_SRC})
target_include_directories(iom PUBLIC ${ZCPKGS_ROOT}/pkgs/)
target_link_libraries(iom PUBLIC zpkg llhttp pthread)