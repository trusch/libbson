cmake_minimum_required (VERSION 2.8.11)

project (libbson)

set( CMAKE_COLOR_MAKEFILE ON )
include_directories( ${CMAKE_SOURCE_DIR}/src/headers )

add_subdirectory( ${CMAKE_SOURCE_DIR}/test/gtest-1.7.0 )
include_directories( ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} )

set( SRC ${CMAKE_SOURCE_DIR}/src/sources/Value.cpp 
				 ${CMAKE_SOURCE_DIR}/src/sources/bson.cpp
				 ${CMAKE_SOURCE_DIR}/src/sources/json.cpp
				 ${CMAKE_SOURCE_DIR}/src/sources/jsmn.cpp )

set( TESTS ${CMAKE_SOURCE_DIR}/test/test1.cpp )

set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall --std=c++11 -fPIC -Os -O3" )

add_library( bson SHARED ${SRC} )
add_library( bson_static STATIC ${SRC} )
set_target_properties( bson_static PROPERTIES OUTPUT_NAME bson )

add_executable( bson_test ${TESTS} )
target_link_libraries( bson_test bson )
target_link_libraries( bson_test gtest )
target_link_libraries( bson_test gtest_main )

install(TARGETS bson bson_static
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
install(DIRECTORY src/headers/ DESTINATION include/bson
        FILES_MATCHING PATTERN "Value.h")
