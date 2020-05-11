/*
Boiler
Copyright (C) 2015  Ansas Bogdan

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CSGOClient_h__
#define CSGOClient_h__
#include <vector>
#include <map>
#include <mutex>
#include <memory>
#include <condition_variable>

#include <steam/steam_api.h>
#include <steam/isteamgamecoordinator.h>
#include "cstrike15_gcmessages.pb.h"
#include "gcsdk_gcmessages.pb.h"
#include "gcsystemmsgs.pb.h"

#include "GCMsgHandler.h"

/**
* emulates the csgo client
*/
class CSGOClient
{
public:
	/**
	* retrieves the instance to the current csgo client or creates a new one
	*/
	static CSGOClient* GetInstance();

	/**
	* destroys the current csgo client
	*/
	static void Destroy();

	/**
	* sends a gc protobuf message
	*/
	EGCResults SendGCMessage(uint32 uMsgType, google::protobuf::Message* msg);

	/**
	* registers a gc protobuf msg handler
	*/
	void RegisterHandler(uint32 msgId, IGCMsgHandler* handler);

	/**
	* removes a gc probouf msg handler
	*/
	void RemoveHandler(uint32 msgId, IGCMsgHandler* handler);

	/**
	* blocks until we are connected to the gc
	*/
	void WaitForGcConnect();

private:
	/**
	* sends client mm hello
	*/
	CSGOClient();
	CSGOClient(const CSGOClient&) = delete;

	/**
	* steam callback for gc messages
	*/
	void OnMessageAvailable(GCMessageAvailable_t* msg);

	/**
	* steam callback for gc messages
	*/
	void OnMessageFailed(GCMessageFailed_t* msg);

	/**
	* handles the gc welcome msg
	*/
	void OnClientWelcome(const CMsgClientWelcome& msg);

private:
	static CSGOClient* m_instance;

	GCMsgHandler<CMsgClientWelcome> m_welcomeHandler;
	std::condition_variable m_connectedCV;
	std::mutex m_connectedMutex;
	bool m_connectedToGc = false;

	ISteamGameCoordinator* m_gameCoordinator;

	CCallback<CSGOClient, GCMessageAvailable_t, false> m_availableCb;
	CCallback<CSGOClient, GCMessageFailed_t, false> m_failedCb;
	std::vector<char> m_recvBuffer;
	std::vector<char> m_msgBuffer;
	std::mutex m_sendMutex;
	std::mutex m_recvMutex;
	std::mutex m_handlerMutex;
	std::multimap<uint32, IGCMsgHandler*> m_msgHandler;
};

#endif // CSGOClient_h__
