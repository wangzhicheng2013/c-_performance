cmake_minimum_required (VERSION 2.8)
project (demo)
include_directories (./func0 ./func1)
aux_source_directory (./func0 SRC_LIST0)
aux_source_directory (./func1 SRC_LIST1)
add_executable (main main.cpp ${SRC_LIST0} ${SRC_LIST1})
