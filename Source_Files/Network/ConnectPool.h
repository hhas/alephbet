#ifndef __CONNECTPOOL_H
#define __CONNECTPOOL_H

/*
 *
 *  Aleph Bet is copyright ©1994-2024 Bungie Inc., the Aleph One developers,
 *  and the Aleph Bet developers.
 *
 *  Aleph Bet is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation, either version 3 of the License, or (at your
 *  option) any later version.
 *
 *  Aleph Bet is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 *  more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 *  This license notice applies only to the Aleph Bet engine itself, and
 *  does not apply to Marathon, Marathon 2, or Marathon Infinity scenarios
 *  and assets, nor to elements of any third-party scenarios.
 *
 */

#include "cseries.h"
#include "CommunicationsChannel.h"
#include <string>
#include <memory>
#include <SDL2/SDL_thread.h>

class NonblockingConnect
{
public:
	NonblockingConnect(const std::string& address, uint16 port);
	NonblockingConnect(const IPaddress& ip);
	~NonblockingConnect();

	enum Status
	{
		Connecting,
		Connected,
		ResolutionFailed,
		ConnectFailed
	};

	Status status() { return m_status; }
	bool done() { return m_status != Connecting; }
	const IPaddress& address() { 
		assert(m_status != Connecting && m_status != ResolutionFailed); 
		return m_ip;
	}
	
	CommunicationsChannel* release() { 
		assert(m_status == Connected); 
		return m_channel.release();
	}


private:
	void connect();
	std::unique_ptr<CommunicationsChannel> m_channel;
	Status m_status;

	std::string m_address;
	uint16 m_port;

	bool m_ipSpecified;
	IPaddress m_ip;

	int Thread();
	static int connect_thread(void *);
	SDL_Thread *m_thread;
};	
		

class ConnectPool
{
public:
	static ConnectPool *instance() { 
		static ConnectPool *m_instance = nullptr;
		if (!m_instance) {
			m_instance = new ConnectPool(); 
		}
		return m_instance; 
	}
	NonblockingConnect* connect(const std::string& address, uint16 port);
	NonblockingConnect* connect(const IPaddress& ip);
	void abandon(NonblockingConnect*);
	~ConnectPool();

private:
	ConnectPool();
	void fast_free();
	enum { kPoolSize = 20 };
	// second is false if we are in use!
	std::pair<NonblockingConnect *, bool> m_pool[kPoolSize];

};

#endif
