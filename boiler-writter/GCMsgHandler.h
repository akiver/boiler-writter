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

#ifndef GCMsgHandler_h__
#define GCMsgHandler_h__
#include <functional>

struct IGCMsgHandler
{
	virtual void Handle(void* buf, size_t len) const = 0;
};

template<typename M>
/**
* class to handle protobuf messages
*/
class GCMsgHandler : public IGCMsgHandler
{
public:
	using cb_t = std::function<void(const M& msg)>;

	template<typename C>
	/**
	* construct from class handler
	*/
	GCMsgHandler(C* instance, void(C::*handler)(const M&))
	{
		m_handler = std::bind(std::mem_fn(handler), instance, std::placeholders::_1);
	}

	template<typename F>
	/**
	* construct from functor
	*/
	GCMsgHandler(const F& handler)
		:m_handler(handler)
	{}

	/**
	* trys to parse the msg from the buffer and calls the handler
	*/
	virtual void Handle(void* buf, size_t len) const final
	{
		M msg;
		msg.ParseFromArray(buf, len);
		m_handler(msg);
	}

private:
	cb_t m_handler;
};

#endif // GCMsgHandler_h__
