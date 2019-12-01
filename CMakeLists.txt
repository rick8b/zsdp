cmake_minimum_required(VERSION 3.10)

project( zsdp )

set(LIB_BASE_NAME "zsdp" CACHE STRING "Library name")

set(STATIC_LIB ${LIB_BASE_NAME})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Set default install prefix
if(NOT WIN32)
   set( CMAKE_INSTALL_PREFIX /usr/local )
endif()

set( PROJ_HEADER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/headers" )
include_directories( "${PROJ_HEADER_DIR}" SYSTEM )
file( GLOB HEADERS "${PROJ_HEADER_DIR}/${LIB_BASE_NAME}/*.h" )
file( GLOB PRIV_HEADERS "*.h" )
file( GLOB CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" )

set(SRCS
    ${CPP_FILES}
    ${HEADERS}
    ${PRIV_HEADERS}
)

add_library( ${STATIC_LIB} STATIC ${SRCS} )
target_link_libraries( ${STATIC_LIB} ${LIBS_DEPENDENCIES} )

include(GenerateExportHeader)
generate_export_header(${LIB_BASE_NAME}
                       BASE_NAME ${LIB_BASE_NAME}
                       EXPORT_MACRO_NAME ZSDP_PUBLIC
					   NO_EXPORT_MACRO_NAME ZSDP_LOCAL
                       STATIC_DEFINE ${LIB_BASE_NAME}_STATIC_DEFINE
					   DEPRECATED_MACRO_NAME ZSDP_DEPRECATED
                       EXPORT_FILE_NAME
					   ${PROJ_HEADER_DIR}/${LIB_BASE_NAME}/exports.h)

set( INSTALLED_TARGETS ${DYNAMIC_LIB} ${STATIC_LIB} )

install( TARGETS ${INSTALLED_TARGETS}
    EXPORT ${LIB_BASE_NAME}
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib )

install(DIRECTORY headers/ DESTINATION include)

install( EXPORT ${LIB_BASE_NAME}
         DESTINATION lib/cmake/${LIB_BASE_NAME}
         FILE ${LIB_BASE_NAME}-config.cmake
)

add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/tests )
