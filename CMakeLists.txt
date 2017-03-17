cmake_minimum_required ( VERSION 3.7.2 )

project ( Unpack VERSION 0.1 LANGUAGES CXX )
set (CMAKE_CXX_STANDARD 14)

include ( CTest )

set ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib" )
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib" )
set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin" )

set ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/Debug/lib" )
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/Debug/lib" )
set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/Debug/bin" )

include_directories (
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_subdirectory ( "./test" )