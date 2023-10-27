# boiler-writter

## About

This is a console application based on [Boiler](https://bitbucket.org/ACB/boiler/).
It retrieve the CMsgGCCStrike15_v2_MatchList protobuf message from Steam GC and write it into the file provided as argument.
Nothing more.

## Why?

I made this application to add the ability to download last matchmaking demos from the application [CSGO Demos Manager](https://github.com/akiver/CSGO-Demos-Manager).
As [Steamworks.NET](http://steamworks.github.io/) doesn't provide a way to communicate with the Steam Game Coordinator, I had to use the C++ Steamworks version.

## Usage

Ready to use executable for each platform can be downloaded from [GitHub](https://github.com/akiver/boiler-writter/releases).

```
./boiler-writter path_to_the_file_where_data_will_be_written [matchId outcomeId tokenId]
```

The 3 optionals parameters are from a [CMsgGCCStrike15_v2_MatchListRequestFullGameInfo](https://github.com/SteamDatabase/Protobufs/blob/eeb5c60e9a6bf9f989b86bf77ec3d9e04a1bb8c6/csgo/cstrike15_gcmessages.proto#L823) message. If they are specified, it will write the `CMsgGCCStrike15_v2_MatchList` message for this specific match.

## Build

Build tested on Windows 10, MAC OS (Catalina) and Linux (Ubuntu 20).

### Common tasks

1. Download the repo and extract it
2. Download the [Steamworks SDK](https://partner.steamgames.com/)
3. Extract it into the solution root folder and rename the folder to `steamworks_sdk`
4. Download the [protobuf 3.20.3](https://github.com/protocolbuffers/protobuf/releases/tag/v3.20.3) source code
5. Extract it into the root folder and make sure the folder's name is `protobuf`
6. Install [CMake](https://cmake.org/download/)

### Windows

1. Download the [protoc 3.20.3 precompiled Windows binary](https://github.com/protocolbuffers/protobuf/releases/download/v3.20.3/protoc-3.20.3-win32.zip) and extract it in a folder included in your `PATH` env variable
2. Ensure `protoc` is installed by running `protoc --version` from a cmd
3. Launch the protobuf solution `protobuf/vsprojects/protobuf.sln` in Visual Studio and migrate projects if required
4. Add `_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS` in `libprotobuf Project properties -> C/C++ -> Preprocessor Definitions` (Debug and / or Release config)
5. Add `#include <algorithm>` in the file `zero_copy_stream_impl_lite.h`
6. Compile the project **libprotobuf** (Using Debug and / or Release)
7. `cmake . -DCMAKE_GENERATOR_PLATFORM=win32` (Add `-DCMAKE_BUILD_TYPE="Release"` to generate the project for Release build)
8. `cmake --build .` (Add `--config Release` if building for Release build)
9. Executable will be in the `bin` folder

A VS solution `BoilerWritter.sln` is also available.

### MAC OS

> **Note**  
> To build the x86_64 version from an arm64 mac, run `/usr/bin/arch -x86_64 /bin/zsh ---login` before running the commands below.

1. Install protobuf, from the folder `protobuf` run:
   1. `brew install automake autoconf libtool`
   2. `./autogen.sh`
   3. `./configure CXXFLAGS="-DNDEBUG"`
   4. `make -j$(sysctl -n hw.physicalcpu)`
   5. `sudo make install`
2. Check if it has been installed by running `protoc --version`
3. From the root project folder: `cmake .` (add `-DCMAKE_BUILD_TYPE="Release"` for release build)
4. `cmake --build .`
5. Executable will be in the `bin` folder

If you want to generate an Xcode project `cmake -G Xcode`.

### Linux

1. `sudo apt install build-essential autoconf gcc gcc-multilib g++-multilib libtool`
2. Install protobuf, from the folder `protobuf` run:
   1. `autoreconf -f -i -Wall,no-obsolete`
   2. `./configure`
   3. `make`
   4. `sudo make install`
   5. `sudo ldconfig`
3. `cmake .` (add `-DCMAKE_BUILD_TYPE="Release"` for release build)
4. `cmake --build .`
5. Executable will be in the `bin` folder

## License

It uses the original [Boiler](https://bitbucket.org/ACB/boiler/) license [GPL v3](https://github.com/akiver/boiler-writter/blob/main/license.txt)
