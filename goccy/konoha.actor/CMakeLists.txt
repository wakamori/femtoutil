cmake_minimum_required(VERSION 2.6)
set(CMAKE_C_FLAGS_RELEASE "-Wall -O2 -g")
set(CMAKE_C_FLAGS_DEBUG   "-Wall -O0 -g3")
set(CMAKE_BUILD_TYPE Release)

if(USE_ACTOR)
project(actor)
set(PACKAGE_SOURCE_CODE src/actor.cpp src/stream.cpp)

set(PACKAGE_SCRIPT_CODE actor.k)

set(PACKAGE_NAME    ${PROJECT_NAME})
set(PACKAGE_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
set(PACKAGE_STRING  ${CMAKE_PROJECT_NAME}-${PACKAGE_VERSION})
set(KONOHA_PACKAGE_DIR konoha/package/${KONOHA_VERSION}/konoha.${PROJECT_NAME})

set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/include  ${CMAKE_CURRENT_BINARY_DIR}
		/usr/lib/jvm/java-6-openjdk/include
		${KONOHA_INCLUDE_DIRS})
include_directories(${INCLUDE_DIRS})

add_definitions(-D_SETUP)

add_library(${PACKAGE_NAME} SHARED ${PACKAGE_SOURCE_CODE})
set_target_properties(${PACKAGE_NAME} PROPERTIES PREFIX "")
target_link_libraries(${PACKAGE_NAME} konoha pthread memcached msgpack)

install(TARGETS ${PACKAGE_NAME} DESTINATION ${KONOHA_PACKAGE_DIR})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE_SCRIPT_CODE} DESTINATION ${KONOHA_PACKAGE_DIR})

endif(USE_ACTOR)
