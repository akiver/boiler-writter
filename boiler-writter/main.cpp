#include <steam/steam_api.h>
#include <fstream>
#include <iostream>
#include "CSGOMatchList.h"

// Retrieve the match list data from Steam GC and write its data to a file
// Args[1]: File path where the CMsgGCCStrike15_v2_MatchList message will be written
// Possible values returned:
// -1 No output file path
// -2 Steam needs to be restarted
// -3 Steam isn't running or not logged on
// -4 Steam communication trouble
// -5 Error while communicating with Steam GC
// -6 Error while getting Matches information
// -7 Error while serializing data
// 0  OK
int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cerr << "boiler.exe path_to_output_file" << std::endl;
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
	});

	try
	{
		// make sure we are connected to the gc
		CSGOClient::GetInstance()->WaitForGcConnect();

		// refresh match list
		CSGOMatchList refresher;
		refresher.RefreshWait();

		CMsgGCCStrike15_v2_MatchList matchListData = refresher.GetMatchListData();
		// save data into a file
		std::string fileDestination(argv[1]);
		std::ofstream file;
		file.open(fileDestination, std::ios::binary);
		if (!matchListData.SerializeToOstream(&file)) {
			return -7;
		}
		file.close();
	}
	catch (std::exception &e) {
		return -6;
	}

	// shutdown
	running = false;
	cbthread.join();
	CSGOClient::Destroy();
	SteamAPI_Shutdown();

	return 0;
}