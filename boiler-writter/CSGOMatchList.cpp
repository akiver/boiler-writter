#include "CSGOMatchList.h"
#include "BoilerException.h"

CSGOMatchList::CSGOMatchList()
	:m_matchListHandler(this, &CSGOMatchList::OnMatchList)
{
	m_onlySpecificMatch = false;
	CSGOClient::GetInstance()->RegisterHandler(k_EMsgGCCStrike15_v2_MatchList, &m_matchListHandler);
}

CSGOMatchList::CSGOMatchList(uint64 matchId, uint64 outcomeId, uint32 tokenId)
	: m_matchListHandler(this, &CSGOMatchList::OnMatchList)
{
	m_onlySpecificMatch = true;
	m_matchId = matchId;
	m_outcomeId = outcomeId;
	m_tokenId = tokenId;
	CSGOClient::GetInstance()->RegisterHandler(k_EMsgGCCStrike15_v2_MatchList, &m_matchListHandler);
}

CSGOMatchList::~CSGOMatchList()
{
	CSGOClient::GetInstance()->RemoveHandler(k_EMsgGCCStrike15_v2_MatchList, &m_matchListHandler);
}

void CSGOMatchList::OnMatchList(const CMsgGCCStrike15_v2_MatchList& msg)
{
	std::unique_lock<std::mutex> lock(m_matchMutex);
	m_matchListData = msg;
	m_updateComplete = true;
	lock.unlock();
	m_updateCv.notify_all();
}

void CSGOMatchList::Refresh()
{
	if (m_onlySpecificMatch) {
		CMsgGCCStrike15_v2_MatchListRequestFullGameInfo request;
		request.set_matchid(m_matchId);
		request.set_outcomeid(m_outcomeId);
		request.set_token(m_tokenId);
		if (CSGOClient::GetInstance()->SendGCMessage(k_EMsgGCCStrike15_v2_MatchListRequestFullGameInfo, &request) != k_EGCResultOK)
			throw BoilerException(BoilerExitCode::CommunicationFailure, "Failed to send k_EMsgGCCStrike15_v2_MatchListRequestFullGameInfo");
	}
	else {
		CMsgGCCStrike15_v2_MatchListRequestRecentUserGames request;
		request.set_accountid(SteamUser()->GetSteamID().GetAccountID());
		if (CSGOClient::GetInstance()->SendGCMessage(k_EMsgGCCStrike15_v2_MatchListRequestRecentUserGames, &request) != k_EGCResultOK)
			throw BoilerException(BoilerExitCode::CommunicationFailure, "Failed to send EMsgGCCStrike15_v2_MatchListRequestRecentUserGames");
	}
}

void CSGOMatchList::RefreshWait()
{
	m_updateComplete = false;
	std::unique_lock<std::mutex> lock(m_matchMutex);
	// wait a bit before sending the refreshing otherwise we may not get the response
	m_updateCv.wait_for(lock, std::chrono::seconds(1));
	Refresh();
	
	std::cv_status status = m_updateCv.wait_for(lock, std::chrono::seconds(10));
	if (status == std::cv_status::timeout) {
		throw BoilerException(BoilerExitCode::AlreadyConnectedToGC, "Already connected to GC, matches can't be received");
	}
}

const CMsgGCCStrike15_v2_MatchList& CSGOMatchList::GetMatchListData() const {
	std::lock_guard<std::mutex> lock(m_matchMutex);
	return m_matchListData;
}
