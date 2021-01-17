cmake_minimum_required (VERSION 3.16)
#set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project (cream_web_server)

LIST(APPEND SRC_DIR commlib frame src build/stubs test)
foreach(cur_dir ${SRC_DIR})
	file(GLOB_RECURSE SRCS ${cur_dir}/*.cpp)
	file(GLOB_RECURSE SRCC ${cur_dir}/*.cc)
	file(GLOB_RECURSE HDRS ${cur_dir}/*.h)
	file(GLOB_RECURSE HPPS ${cur_dir}/*.hpp)
	LIST(APPEND SRC_LIST ${SRCS} ${SRCC})
	LIST(APPEND HDR_LIST ${HDRS} ${HPPS})
endforeach()
add_compile_options(-std=c++11)
add_executable(cream_web_server main.cpp ${SRC_LIST} ${HDR_LIST})
install (TARGETS cream_web_server DESTINATION bin)

SET(PROJECT_ROOT_PATH ".")   
INCLUDE_DIRECTORIES("${PROJECT_ROOT_PATH}")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED)
include_directories(/usr/include/mysql)
include_directories(/usr/local/include/mysql++)

target_include_directories(cream_web_server
	PRIVATE ${PROTOBUF_INCLUDE_DIR}
)
target_link_libraries(cream_web_server
	PRIVATE ${PROTOBUF_LIBRARY}
	PRIVATE mysqlpp
	PRIVATE Threads::Threads
	PUBLIC ${COMM_LIB}
)

SET(PROTO_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src")
SET(PROJECT_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build")
SET(PROTO_META_BASE_DIR "${PROJECT_BUILD_DIR}/stubs")
file(GLOB protobuf_files
	${PROTO_PATH}/*.proto
)
LIST(APPEND PROTO_FLAGS -I${PROTO_PATH})
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/stubs)
FOREACH(FIL ${protobuf_files})
	message("Dealing with ${FIL}")
	EXECUTE_PROCESS(
		COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} ${PROTO_FLAGS} --cpp_out=${PROTO_META_BASE_DIR} ${FIL}
    )
ENDFOREACH()
