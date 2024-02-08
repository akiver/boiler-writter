# boiler-writter

## About

This is a console application based on [Boiler](https://bitbucket.org/ACB/boiler/).
It retrieves the CMsgGCCStrike15_v2_MatchList protobuf message from Steam GC and writes it into the file provided as an argument.
Nothing more.

## Why?

I made this application to add the ability to download last matchmaking demos from the application [CS Demo Manager](https://github.com/akiver/CSGO-Demos-Manager).

## Usage

A ready-to-use executable for each platform can be downloaded from [GitHub](https://github.com/akiver/boiler-writter/releases).

```
./boiler-writter path_to_the_file_where_data_will_be_written [matchId outcomeId tokenId]
```

The 3 optional parameters must come from a [CMsgGCCStrike15_v2_MatchListRequestFullGameInfo](https://github.com/SteamDatabase/Protobufs/blob/eeb5c60e9a6bf9f989b86bf77ec3d9e04a1bb8c6/csgo/cstrike15_gcmessages.proto#L823) message.  
If they are specified, it will write the `CMsgGCCStrike15_v2_MatchList` message for this specific match.

## Build

### Common tasks

1. Download the repo and extract it
2. Download the [Steamworks SDK](https://partner.steamgames.com/)
3. Extract it into the solution root folder and rename the folder to `steamworks_sdk`
4. Download the [protobuf 3.20.3](https://github.com/protocolbuffers/protobuf/releases/tag/v3.20.3) source code
5. Extract it into the root folder and make sure the folder's name is `protobuf`
6. Install [CMake](https://cmake.org/download/)

### Windows

1. Install protobuf - from the root folder run:
   1. `cd protobuf/cmake && mkdir build`
   2. `cmake -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=.\build .`
   3. `cmake --build . --config Release`
   4. `cd ../..`
   5. Add the path `protobuf/cmake/Release` to your `PATH` environment variable (where `protoc.exe` is)
2. `cmake . -DCMAKE_BUILD_TYPE=Release`
3. `cmake --build . --config Release`
4. Executable will be in the `bin` folder

A VS solution `BoilerWritter.sln` is also available.

### MAC OS

> **Note**  
> To build the x86_64 version from an arm64 mac, run `/usr/bin/arch -x86_64 /bin/zsh ---login` before running the commands below.

1. `brew install automake autoconf libtool`
2. Install protobuf - from the root folder run:
   1. `cd protobuf`
   2. `./autogen.sh`
   3. `./configure CXXFLAGS="-DNDEBUG"`
   4. `make -j$(sysctl -n hw.physicalcpu)`
   5. `sudo make install`
   6. `cd ..`
3. `cmake . -DCMAKE_BUILD_TYPE=Release`
4. `cmake --build . --config Release`
5. Executable will be in the `bin` folder

If you want to generate an Xcode project `cmake -G Xcode`.

### Linux

1. `sudo apt install build-essential autoconf gcc gcc-multilib g++-multilib libtool cmake`
2. Install protobuf - from the root folder run:
   1. `cd protobuf`
   2. `./autogen.sh`
   3. `./configure CXXFLAGS="-DNDEBUG"`
   4. `make`
   5. `sudo make install`
   6. `sudo ldconfig`
   7. `cd ..`
3. `cmake . -DCMAKE_BUILD_TYPE=Release`
4. `cmake --build . --config Release`
5. Executable will be in the `bin` folder

## License

It uses the original [Boiler](https://bitbucket.org/ACB/boiler/) license [GPL v3](https://github.com/akiver/boiler-writter/blob/main/license.txt)
