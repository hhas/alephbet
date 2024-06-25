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
#include "Scenario.h"
#include "InfoTree.h"

Scenario *Scenario::instance()
{
	static Scenario *m_instance = nullptr;
	if (!m_instance) m_instance = new Scenario();
	return m_instance;
}

void Scenario::AddCompatible(const string Compatible)
{
	m_compatibleVersions.push_back(string(Compatible, 0, 23));
}

bool Scenario::IsCompatible(const string Compatible)
{
	if (Compatible == "" || m_id == "") return true;
	if (Compatible == m_id) return true;
	for (int i = 0; i < m_compatibleVersions.size(); i++)
	{
		if (m_compatibleVersions[i] == Compatible) return true;
	}

	return false;
}


void reset_mml_scenario()
{
	// no reset
}

void parse_mml_scenario(const InfoTree& root)
{
	std::string str;
	if (root.read_attr("name", str))
		Scenario::instance()->SetName(str);
	if (root.read_attr("id", str))
		Scenario::instance()->SetID(str);
	if (root.read_attr("version", str))
		Scenario::instance()->SetVersion(str);
	
	for (const InfoTree &can_join : root.children_named("can_join"))
	{
		std::string compat = can_join.get_value("");
		if (compat.size())
			Scenario::instance()->AddCompatible(compat);
	}
}
