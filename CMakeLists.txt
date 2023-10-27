cmake_minimum_required(VERSION 3.17.2 FATAL_ERROR)
project(BoilerWritter)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INSTALL_RPATH "$ORIGIN")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread --std=c++14")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(Protobuf_USE_STATIC_LIBS ON)
set(Protobuf_SRC_ROOT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/protobuf)

include(FindPackageHandleStandardArgs)

find_package(Steamworks REQUIRED)

if (WIN32)
    if(CMAKE_BUILD_TYPE MATCHES Release)
        set(PROTOBUF_LIB_DIR Release)
        set(PROTOBUF_LIB_NAME libprotobuf)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
    else()
        set(PROTOBUF_LIB_DIR Debug)
        set(PROTOBUF_LIB_NAME libprotobufd)
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
    endif()

    set(Protobuf_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/cmake/${PROTOBUF_LIB_DIR}/${PROTOBUF_LIB_NAME}.lib)
endif()

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
    boiler-writter/BoilerExitCode.h
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

include_directories(${STEAMWORKS_INCLUDE_DIR})
include_directories(${Protobuf_INCLUDE_DIRS})
target_link_libraries(boiler-writter ${Protobuf_LIBRARIES} ${STEAMWORKS_LIBRARY})

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

file(COPY "steam_appid.txt" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

if (WIN32) 
    file(COPY ${STEAMWORKS_SDK_PATH}/${STEAMWORKS_BIN_PATH}/steam_api64.dll DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    add_custom_command(TARGET boiler-writter POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/boiler-writter.exe ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    add_custom_command(TARGET boiler-writter POST_BUILD COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE})
else()
    file(COPY ${STEAMWORKS_LIBRARY} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

