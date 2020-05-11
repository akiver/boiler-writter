# boiler-writter

## About

This is a console application based on [Boiler](https://bitbucket.org/ACB/boiler/).
It retrieve the CMsgGCCStrike15_v2_MatchList protobuf message from Steam GC and write it into the file provided as argument.
Nothing more.

## Why?

I made this application to add the ability to download last matchmaking demos from the application [CSGO Demos Manager](https://github.com/akiver/CSGO-Demos-Manager).
As [Steamworks.NET](http://steamworks.github.io/) doesn't provide a way to communicate with the Steam Game Coordinator, I had to use the C++ Steamworks version.

## Usage

```
./boiler-writter path_to_the_file_where_data_will_be_written [matchId outcomeId tokenId]
```

The 3 optionals parameters are from a [CMsgGCCStrike15_v2_MatchListRequestFullGameInfo](https://github.com/SteamRE/SteamKit/blob/master/Resources/Protobufs/csgo/cstrike15_gcmessages.proto#L744) message. If they are specified, it will write the CMsgGCCStrike15_v2_MatchList message for this specific match.

## Build

Build tested on Windows, MAC OS (Sierra) and Linux.

### Common tasks

1. Download the repo and extract it
2. Download the [Steamworks SDK](https://partner.steamgames.com/)
3. Extract it into the solution root folder and rename the folder to "steamworks_sdk"
4. Download the [protobuf 2.5.0](https://github.com/google/protobuf/releases/tag/v2.5.0) sources
5. Extract it into the solution root folder

### Windows

1. Launch the protobuf solution in Visual Studio
2. Compile the project "libprotobuf" (Debug or / and Release)
3. Launch the boiler-writter solution
4. Build

### MAC OS

1. Install Protobuf from sources, steps are in the README but you should do:
   1. `brew install automake autoconf libtool`
   2. `./autogen.sh` (You may have to run `LANG=C LC_CTYPE=C ./autogen.sh` instead of `./autogen.sh`)
   3. `./configure`
   4. `make`
   5. `make install`
2. Check if it has been installed by running `protoc --version`.
   By default header files should have been installed in `/usr/local/include` and lib in `/usr/local/lib`. This is where Xcode assume that they are located but you can change it if you want from Xcode.
3. Launch the project in Xcode
4. Make sure `libsteam_api.dylib` and `libprotobuf.8.dylib` are in the **Linked Frameworks and Libraries** from _General_ tab.
5. Build the project

### Linux

**Tested on Ubuntu 19.10**

1. Install [CMake](https://cmake.org/download/)
2. `sudo apt install gcc gcc-multilib g++-multilib`
3. Install protobuf 2.5.0 from source
   1. `wget https://github.com/protocolbuffers/protobuf/releases/download/v2.5.0/protobuf-2.5.0.tar.gz`
   2. `tar -zxvf protobuf-2.5.0.tar.gz && cd protobuf-2.5.0`
   3. `./configure`
   4. `make`
   5. `make install`
   6. `sudo ldconfig`
4. `cmake . -B build` (add `-DCMAKE_BUILD_TYPE="Release"` for release build)
5. `cmake --build build`
6. Executable will be in the `build` folder

## License

It uses the original [Boiler](https://bitbucket.org/ACB/boiler/) license [GPL v3](https://github.com/akiver/boiler-writter/blob/master/license.txt)
