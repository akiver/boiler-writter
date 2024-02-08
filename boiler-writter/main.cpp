#include <steam/steam_api.h>
#include <thread>
#include <fstream>
#include <iostream>
#include "CSGOMatchList.h"
#include "BoilerException.h"

int process(CSGOMatchList& refresher, char* outputPath) {
	// refresh match list
	refresher.RefreshWait();

	CMsgGCCStrike15_v2_MatchList matchListData = refresher.GetMatchListData();

	if (matchListData.matches_size() == 0) {
		return (int)BoilerExitCode::NoMatchesFound;
	}
	std::string fileDestination(outputPath);
	std::ofstream file;
	file.open(fileDestination, std::ios::binary);
	if (!matchListData.SerializeToOstream(&file)) {
		return (int)BoilerExitCode::WriteFileFailure;
	}
	file.close();

	return (int)BoilerExitCode::Success;
}

// Retrieve the match list data from Steam GC and write its data to a file
// Args[1]: File path where the CMsgGCCStrike15_v2_MatchList message will be written
// The next 3 args are optionals but all are required if argc > 2
// They correspond to the fields from the proto message CMsgGCCStrike15_v2_MatchListRequestFullGameInfo
// https://github.com/SteamRE/SteamKit/blob/master/Resources/Protobufs/csgo/cstrike15_gcmessages.proto#L744
// Args[2]: optional matchid
// Args[3]: optional outcomeid
// Args[4]: optional tokenid
// The process exit with a specific code, see the enum BoilerExitCode for possible values.
int main(int argc, char *argv[]) {
	bool isRecentMatchRequest = argc == 2;
	if (!isRecentMatchRequest && argc != 5) {
		std::cout << "boiler.exe path_to_output_file [matchid outcomeid tokenid]" << std::endl;
		return (int)BoilerExitCode::InvalidArgs;
	}
	if (SteamAPI_RestartAppIfNecessary(k_uAppIdInvalid)) {
		return (int)BoilerExitCode::SteamRestartRequired;
	}
	if (!SteamAPI_Init()) {
		return (int)BoilerExitCode::SteamNotRunningOrLoggedIn;
	}
	if (!SteamUser()->BLoggedOn()) {
		return (int)BoilerExitCode::SteamUserNotLoggedIn;
	}

	bool running = true;
	auto cbthread = std::thread([&running]() {
		while (running) {
			try {
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				SteamAPI_RunCallbacks();
			}
			catch (BoilerException &e) {
				fprintf(stderr, e.what());
				exit(e.getExitCode());
			}
			catch (std::exception& e) {
				fprintf(stderr, "Fatal error:\n");
				fprintf(stderr, e.what());
				exit((int)BoilerExitCode::FatalError);
			}
		};
	});

	int result = 0;

	try
	{
		printf("STEAMID:%lld\n", SteamUser()->GetSteamID().ConvertToUint64());

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
	catch (BoilerException& e) {
		fprintf(stderr, e.what());
		result = e.getExitCode();
	}
	catch (std::exception& e) {
		fprintf(stderr, "Fatal error:\n");
		fprintf(stderr, e.what());
		result = (int)BoilerExitCode::FatalError;
	}

	// shutdown
	running = false;
	cbthread.join();
	CSGOClient::Destroy();
	SteamAPI_Shutdown();

	return result;
}