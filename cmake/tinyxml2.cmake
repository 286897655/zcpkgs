#slivesdkstream use tinyxml2 for parse sdk xm value

SET(TINYXML2_ROOT ${PROJECT_ROOT}/import/tinyxml2)

add_library(tinyxml2 ${TINYXML2_ROOT}/tinyxml2.cpp ${TINYXML2_ROOT}/tinyxml2.h)
target_include_directories(tinyxml2 PUBLIC ${TINYXML2_ROOT}/)

# add tinyxml2 to link list
# list(APPEND APPEND_TARGET_LIB tinyxml2)