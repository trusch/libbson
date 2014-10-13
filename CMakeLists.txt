# CMakeLists files in this project can
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and
# to the root binary directory of the project as ${HELLO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.11)

project (libbson)

set( CMAKE_COLOR_MAKEFILE ON )
include_directories( ${CMAKE_SOURCE_DIR}/src/headers )

set( SRC ${CMAKE_SOURCE_DIR}/src/sources/Value.cpp 
				 ${CMAKE_SOURCE_DIR}/src/sources/bson.cpp
				 ${CMAKE_SOURCE_DIR}/src/sources/json.cpp
				 ${CMAKE_SOURCE_DIR}/src/sources/jsmn.cpp )

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall --std=c++11 -fPIC -O3 -g" )

add_library (bson SHARED ${SRC})
add_library (bson_static STATIC ${SRC})
set_target_properties(bson_static PROPERTIES OUTPUT_NAME bson)

install(TARGETS bson bson_static
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
install(DIRECTORY src/headers/ DESTINATION include/bson
        FILES_MATCHING PATTERN "Value.h")
