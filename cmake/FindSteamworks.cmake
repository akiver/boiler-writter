set(STEAMWORKS_SDK_PATH "${CMAKE_SOURCE_DIR}/steamworks_sdk/sdk")

if (APPLE)
	set(STEAMWORKS_BIN_PATH "redistributable_bin/osx")
elseif(WIN32)
	set(STEAMWORKS_BIN_PATH "redistributable_bin")
else()
	set(STEAMWORKS_BIN_PATH "redistributable_bin/linux64")
endif ()

find_path(STEAMWORKS_INCLUDE_DIR
	NAMES steam/steam_api.h
	PATH_SUFFIXES "public"
	PATHS ${STEAMWORKS_SDK_PATH}
)

find_library(STEAMWORKS_LIBRARY
	NAMES steam_api
	PATH_SUFFIXES ${STEAMWORKS_BIN_PATH}
	PATHS ${STEAMWORKS_SDK_PATH}
)

find_package_handle_standard_args(Steamworks DEFAULT_MSG STEAMWORKS_LIBRARY STEAMWORKS_INCLUDE_DIR)