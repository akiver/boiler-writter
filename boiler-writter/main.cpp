#include <steam/steam_api.h>
#include <thread>
#include <fstream>
#include <iostream>
#include "CSGOMatchList.h"

int process(CSGOMatchList& refresher, char* outputPath) {
	// refresh match list
	refresher.RefreshWait();

	CMsgGCCStrike15_v2_MatchList matchListData = refresher.GetMatchListData();
	std::cerr << matchListData.matches_size() << std::endl;

	if (matchListData.matches_size() == 0) {
		return -8;
	}
	// save data into a file
	std::string fileDestination(outputPath);
	std::ofstream file;
	file.open(fileDestination, std::ios::binary);
	if (!matchListData.SerializeToOstream(&file)) {
		return -7;
	}
	file.close();

	return 0;
}

// Retrieve the match list data from Steam GC and write its data to a file
// Args[1]: File path where the CMsgGCCStrike15_v2_MatchList message will be written
// The next 3 args are optionals but all are required if argc > 2
// They correspond to the fields from the proto message CMsgGCCStrike15_v2_MatchListRequestFullGameInfo
// https://github.com/SteamRE/SteamKit/blob/master/Resources/Protobufs/csgo/cstrike15_gcmessages.proto#L744
// Args[2]: optional matchid
// Args[3]: optional outcomeid
// Args[4]: optional tokenid
// Possible values returned:
// -1 No output file path
// -2 Steam needs to be restarted
// -3 Steam isn't running or not logged on
// -4 Steam communication trouble
// -5 Error while communicating with Steam GC
// -6 Error while getting Matches information
// -7 Error while serializing data
// -8 No matches found
// 0  OK
int main(int argc, char *argv[]) {
	bool isRecentMatchRequest = argc == 2;
	if (!isRecentMatchRequest && argc != 5) {
		std::cerr << "boiler.exe path_to_output_file [matchid outcomeid tokenid]" << std::endl;
		return -1;
	}
	if (SteamAPI_RestartAppIfNecessary(k_uAppIdInvalid)) {
		return -2;
	}
	if (!SteamAPI_Init()) {
		return -3;
	}
	if (!SteamUser()->BLoggedOn()) {
		return -4;
	}

	bool running = true;
	auto cbthread = std::thread([&running]() {
		while (running) {
			try {
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				Steam_RunCallbacks(GetHSteamPipe(), false);
			}
			catch (std::exception &e) {
				return -5;
			}
		};
		return 0;
	});

	int result = 0;

	try
	{
		// make sure we are connected to the gc
		CSGOClient::GetInstance()->WaitForGcConnect();

		if (isRecentMatchRequest) {
			CSGOMatchList refresher;
			result = process(refresher, argv[1]);
		} else {
			uint64 matchId = strtoull(argv[2], NULL, 0);
			uint64 outcomeId = strtoull(argv[3], NULL, 0);
			uint32 tokenId = atoi(argv[4]);
			CSGOMatchList refresher(matchId, outcomeId, tokenId);
			result = process(refresher, argv[1]);
		}
	}
	catch (std::exception &e) {
		return -6;
	}

	// shutdown
	running = false;
	cbthread.join();
	CSGOClient::Destroy();
	SteamAPI_Shutdown();

	return result;
}