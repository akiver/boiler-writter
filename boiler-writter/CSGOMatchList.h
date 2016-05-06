#ifndef CSGOMatchList_h__
#define CSGOMatchList_h__
#include <condition_variable>
#include "CSGOClient.h"


/**
* utility class to store and update the match list
*/
class CSGOMatchList
{
public:

	CSGOMatchList();
	~CSGOMatchList();

	void Refresh();
	void RefreshWait();

	const CMsgGCCStrike15_v2_MatchList& GetMatchListData() const;

private:
	void OnMatchList(const CMsgGCCStrike15_v2_MatchList& msg);

private:
	bool m_updateComplete;
	std::condition_variable m_updateCv;
	mutable std::mutex m_matchMutex;
	GCMsgHandler<CMsgGCCStrike15_v2_MatchList> m_matchListHandler;
	CMsgGCCStrike15_v2_MatchList m_matchListData;
};

#endif // CSGOMatchList_h__
