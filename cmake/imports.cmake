# zcpkgs import
SET(ZCPKGS_IMPORT_ROOT ${PROJECT_ROOT}/import)
add_library(tinyxml2 STATIC ${ZCPKGS_IMPORT_ROOT}/tinyxml2/tinyxml2.cpp ${ZCPKGS_IMPORT_ROOT}/tinyxml2/tinyxml2.h)
target_include_directories(tinyxml2 PUBLIC ${ZCPKGS_IMPORT_ROOT})
add_library(pugixml STATIC ${ZCPKGS_IMPORT_ROOT}/pugixml/pugixml.hpp ${ZCPKGS_IMPORT_ROOT}/pugixml/pugixml.cpp ${ZCPKGS_IMPORT_ROOT}/pugixml/pugiconfig.hpp)
target_include_directories(pugixml PUBLIC ${ZCPKGS_IMPORT_ROOT})
# file(GLOB_RECURSE LLHTTP_SRC ${ZCPKGS_IMPORT_ROOT}/llhttp/*.h
#                              ${ZCPKGS_IMPORT_ROOT}/llhttp/*.c)
add_library(llhttp STATIC ${ZCPKGS_IMPORT_ROOT}/llhttp/llhttp.h ${ZCPKGS_IMPORT_ROOT}/llhttp/llhttp.c)
target_include_directories(llhttp PUBLIC ${ZCPKGS_IMPORT_ROOT})