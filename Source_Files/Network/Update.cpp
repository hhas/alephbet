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

/*
 *  Checks for updates online
 */

#include "Update.h"
#include "HTTP.h"
#include <sstream>
#include <boost/tokenizer.hpp>
#include "alephversion.h"


Update::Update() : m_status(NoUpdateAvailable), m_thread(0)
{
	StartUpdateCheck();
}

Update::~Update()
{
	if (m_thread)
	{
		int status;
		SDL_WaitThread(m_thread, &status);
		m_thread = 0;
	}
}

int Update::update_thread(void *p)
{
	Update *update = static_cast<Update *>(p);
	return update->Thread();
}

void Update::StartUpdateCheck()
{
	if (m_status == CheckingForUpdate) return;
	if (m_thread)
	{
		int status;
		SDL_WaitThread(m_thread, &status);
		m_thread = 0;
	}

	m_status = CheckingForUpdate;
	m_new_date_version.clear();
	m_new_display_version.clear();

	m_thread = SDL_CreateThread(update_thread, "Update_checkThread", this);
	if (!m_thread)
	{
		m_status = UpdateCheckFailed;
	}
}

int Update::Thread()
{
	HTTPClient fetcher;
	if (!fetcher.Get(AB_UPDATE_URL))
	{
		m_status = UpdateCheckFailed;
		return 1;
	}

	boost::char_separator<char> sep("\r\n");
	std::string response = fetcher.Response();
	boost::tokenizer<boost::char_separator<char> > tokens(response, sep);
	for (boost::tokenizer<boost::char_separator<char> >::iterator it = tokens.begin();
	     it != tokens.end();
	     ++it)
	{
		if (starts_with(*it, "AB_DATE_VERSION: "))
		{
			m_new_date_version = it->substr(strlen("AB_DATE_VERSION: "));
		}
		else if (starts_with(*it, "AB_DISPLAY_VERSION: "))
		{
			m_new_display_version = it->substr(strlen("AB_DISPLAY_VERSION: "));
		}
	}

	if (m_new_date_version.size())
	{
		m_status = m_new_date_version.compare(AB_DATE_VERSION) > 0 ? UpdateAvailable : NoUpdateAvailable;
		return 0;
	}
	else
	{
		m_status = UpdateCheckFailed;
		return 5;
	}
}


