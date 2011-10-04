cmake_minimum_required(VERSION 2.6)

set(VERSION_MAJOR "1")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(KONOHA_VERSION  ${VERSION_MAJOR}.${VERSION_MINOR})

set(CMAKE_C_FLAGS_RELEASE "-Wall -O0 -g3")
set(CMAKE_C_FLAGS_DEBUG   "-Wall -O0 -g3")
set(CMAKE_BUILD_TYPE Release)

project(actor)
set(PACKAGE_SOURCE_CODE src/actor.c)
set(PACKAGE_SCRIPT_CODE actor.k)

set(PACKAGE_NAME    ${PROJECT_NAME})
set(PACKAGE_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
set(PACKAGE_STRING  ${CMAKE_PROJECT_NAME}-${PACKAGE_VERSION})
set(KONOHA_PACKAGE_DIR konoha/package/${KONOHA_VERSION}/konoha.${PROJECT_NAME})

set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/include  ${CMAKE_CURRENT_BINARY_DIR}
		${KONOHA_INCLUDE_DIRS} /opt/local/include)
include_directories(${INCLUDE_DIRS})

#find_library(LIB_MEMCACHED memcached)
#if(NOT EXISTS ${LIB_MEMCACHED})
#	message(FATAL_ERROR "libmemcached not found.")
#endif(NOT EXISTS ${LIB_MEMCACHED})
#
#find_library(LIB_MSGPACK msgpack)
#if(NOT EXISTS ${LIB_MSGPACK})
#	message(FATAL_ERROR "libmsgpack not found.")
#endif(NOT EXISTS ${LIB_MSGPACK})

add_definitions(-D_SETUP)

add_library(${PACKAGE_NAME} SHARED ${PACKAGE_SOURCE_CODE})
set_target_properties(${PACKAGE_NAME} PROPERTIES PREFIX "")
#target_link_libraries(${PACKAGE_NAME} konoha pthread ${LIB_MEMCACHED} ${LIB_MSGPACK})
target_link_libraries(${PACKAGE_NAME} konoha)

install(TARGETS ${PACKAGE_NAME} DESTINATION ${KONOHA_PACKAGE_DIR})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE_SCRIPT_CODE} DESTINATION ${KONOHA_PACKAGE_DIR})
