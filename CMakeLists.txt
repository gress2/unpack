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

set ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/Release/lib" )
set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/Release/lib" )
set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/Release/bin" )

set ( CMAKE_CXX_FLAGS_RELEASE "-Wall -O3 -g" )

option (O0 "O0" OFF)
option (O1 "O1" OFF)
option (O2 "O2" OFF)
option (O3 "O3" OFF)
option (Ofast "Ofast" OFF)

if ( O0 )
    set ( CMAKE_CXX_FLAGS_RELEASE "-Wall -O0" )
endif( O0 )
if ( O1 )
    set ( CMAKE_CXX_FLAGS_RELEASE "-Wall -O1" )
endif( O1 )
if ( O2 )
    set ( CMAKE_CXX_FLAGS_RELEASE "-Wall -O2" )
endif( O2 )
if ( O3 )
    set ( CMAKE_CXX_FLAGS_RELEASE "-Wall -O3" )
endif( O3 )
if ( Ofast )
    set ( CMAKE_CXX_FLAGS_RELEASE "-Wall -Ofast" )
endif( Ofast )

include_directories (
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_subdirectory ( "./test" )
add_subdirectory ( "./benchmark" )
add_subdirectory ( "./deps/googletest" )
option ( BENCHMARK_ENABLE_TESTING OFF ) 
add_subdirectory ( "./deps/benchmark" )
