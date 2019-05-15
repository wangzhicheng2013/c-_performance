aux_source_directory (. SRC_LIST)
include_directories (../include)
add_executable (client_status_manager ${SRC_LIST})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17")
find_package(Boost COMPONENTS system REQUIRED)
include_directories(
	"../include/rest_rpc/include"
	"../include/rest_rpc/third/msgpack/include"
	"../include/nanolog"
	"../include/doctest"
)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
target_link_libraries(client_status_manager ${Boost_LIBRARIES} -pthread)
option(BUILD_TESTS "If enabled, compile the tests." ON)
if(BUILD_TESTS)
	enable_testing()
	macro(add_doctest test_source test_name)
	add_executable(${test_name} ${test_source})
	add_test(${test_name} ${test_name})
	endmacro(add_doctest)
	add_subdirectory(tests)
endif()
