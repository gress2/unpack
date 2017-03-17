include_directories( include )
add_executable ( test1 test1.cpp )

add_test ( NAME test1 COMMAND test1 )
