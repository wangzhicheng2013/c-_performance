aux_source_directory (. SRC_LIST)
link_directories (${PROJECT_SOURCE_DIR}/lib)
add_executable (main ${SRC_LIST})
target_link_libraries (main func)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
