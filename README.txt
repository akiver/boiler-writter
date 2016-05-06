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
boiler.exe path_to_the_file_where_data_will_be_written
```

## Build

As this application has been made for [CSGO Demos Manager](https://github.com/akiver/CSGO-Demos-Manager), it has been tested only on Windows.

1. Download the repo and extract it
2. Download the [protobuf 2.5.0](https://github.com/google/protobuf/releases/tag/v2.5.0) src
3. Extract it into the solution root folder
4. Launch the protobuf solution
5. Compile the project "libprotobuf" (Debug or / and Release)
4. Download the [Steamworks SDK](https://partner.steamgames.com/)
5. Extract it into the solution root folder and rename the folder to "steamworks_sdk"
6. Launch the boiler-writter solution
7. Build

## License

It use the original [Boiler](https://bitbucket.org/ACB/boiler/) license [GPL v3](https://github.com/akiver/boiler-writter/blob/master/license.txt)