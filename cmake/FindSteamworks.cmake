set(STEAMWORKS_POSSIBLE_PATHS "${CMAKE_SOURCE_DIR}/steamworks_sdk/sdk")

if (APPLE)
	set(REDISTRIBUABLE_PATH "redistributable_bin/osx")
elseif(WIN32)
	set(REDISTRIBUABLE_PATH "redistributable_bin")
else()
	set(REDISTRIBUABLE_PATH "redistributable_bin/linux64")
endif ()

message(${STEAMWORKS_POSSIBLE_PATHS})

find_path(STEAMWORKS_INCLUDE_DIR
	NAMES steam/steam_api.h
	PATH_SUFFIXES "public"
	PATHS ${STEAMWORKS_POSSIBLE_PATHS}
)

find_library(STEAMWORKS_LIBRARY
	NAMES steam_api
	PATH_SUFFIXES ${REDISTRIBUABLE_PATH}
	PATHS ${STEAMWORKS_POSSIBLE_PATHS}
)

find_package_handle_standard_args(Steamworks DEFAULT_MSG STEAMWORKS_LIBRARY STEAMWORKS_INCLUDE_DIR)