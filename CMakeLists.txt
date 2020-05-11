cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)
project(BoilerWritter)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(FindPackageHandleStandardArgs)

find_package(Steamworks REQUIRED)

find_package(Protobuf REQUIRED)
protobuf_generate_cpp(CSTRIKE_GC_PROTO_SOURCES CSTRIKE_GC_PROTO_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/boiler-writter/protobuf/cstrike15_gcmessages.proto)
protobuf_generate_cpp(STEAM_PROTO_SOURCES STEAM_PROTO_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/boiler-writter/protobuf/steammessages.proto)
protobuf_generate_cpp(GC_PROTO_SOURCES GC_PROTO_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/boiler-writter/protobuf/gcsdk_gcmessages.proto)
protobuf_generate_cpp(GCSYS_PROTO_SOURCES GCSYS_PROTO_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/boiler-writter/protobuf/gcsystemmsgs.proto)
protobuf_generate_cpp(ENGINE_PROTO_SOURCES ENGINE_PROTO_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/boiler-writter/protobuf/engine_gcmessages.proto)

add_executable(boiler-writter
    boiler-writter/main.cpp
    boiler-writter/CSGOClient.h
    boiler-writter/CSGOClient.cpp
    boiler-writter/GCMsgHandler.h
    boiler-writter/CSGOMatchList.h
    boiler-writter/CSGOMatchList.cpp
	boiler-writter/BoilerException.h
	boiler-writter/BoilerException.cpp
    boiler-writter/protobuf/cstrike15_gcmessages.proto
    boiler-writter/protobuf/steammessages.proto
    boiler-writter/protobuf/gcsdk_gcmessages.proto
    boiler-writter/protobuf/gcsystemmsgs.proto
    boiler-writter/protobuf/engine_gcmessages.proto
    ${CSTRIKE_GC_PROTO_SOURCES}
    ${CSTRIKE_GC_PROTO_HEADERS}
    ${STEAM_PROTO_SOURCES}
    ${STEAM_PROTO_HEADERS}
    ${GC_PROTO_SOURCES}
    ${GC_PROTO_HEADERS}
    ${GCSYS_PROTO_SOURCES}
    ${GCSYS_PROTO_HEADERS}
    ${ENGINE_PROTO_SOURCES}
    ${ENGINE_PROTO_HEADERS})

include_directories("${STEAMWORKS_INCLUDE_DIR}" "${PROTOBUF_INCLUDE_DIR}")
target_link_libraries(boiler-writter "${STEAMWORKS_LIBRARY}" ${Protobuf_LIBRARIES})

source_group("Generated Files" FILES 
    ${CSTRIKE_GC_PROTO_SOURCES}
    ${CSTRIKE_GC_PROTO_HEADERS}
    ${STEAM_PROTO_SOURCES}
    ${STEAM_PROTO_HEADERS}
    ${GC_PROTO_SOURCES}
    ${GC_PROTO_HEADERS}
    ${GCSYS_PROTO_SOURCES}
    ${GCSYS_PROTO_HEADERS}
    ${ENGINE_PROTO_SOURCES}
    ${ENGINE_PROTO_HEADERS})

source_group("Protobuf Files" FILES
    boiler-writter/protobuf/cstrike15_gcmessages.proto
    boiler-writter/protobuf/steammessages.proto
    boiler-writter/protobuf/gcsdk_gcmessages.proto
    boiler-writter/protobuf/engine_gcmessages.proto
    boiler-writter/protobuf/gcsystemmsgs.proto)
