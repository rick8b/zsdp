cmake_minimum_required(VERSION 3.10)

project( zsdp-tests )

set ( CMAKE_CXX_STANDARD 11 )

set( PROJ_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../headers" )

add_executable( test-sdp test-sdp.cpp ../sdp.cpp ../parsing.cpp ../string-util.cpp ../net-util.cpp ../attributes.cpp ../attribute-parsing.cpp )
add_test ( NAME test-sdp COMMAND test-sdp )

add_executable( test-enum-parsing test-enum-parsing.cpp ../parsing.cpp ../string-util.cpp ../net-util.cpp ../attributes.cpp ../attribute-parsing.cpp )
add_test ( NAME test-enum-parsing COMMAND test-enum-parsing )

add_executable( test-attribute-parsing test-attribute-parsing.cpp ../parsing.cpp ../string-util.cpp ../net-util.cpp ../attributes.cpp ../attribute-parsing.cpp )
add_test ( NAME test-attribute-parsing COMMAND test-attribute-parsing )

add_executable( test-string-util test-string-util.cpp ../string-util.cpp )
add_test ( NAME test-string-util COMMAND test-string-util )

add_executable( test-net-util test-net-util.cpp ../net-util.cpp )
add_test ( NAME test-net-util COMMAND test-net-util )



include_directories( ${PROJECT_INCLUDE_DIR} )
