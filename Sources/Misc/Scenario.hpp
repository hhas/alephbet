#ifndef _SCENARIO
#define _SCENARIO

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

#include <string>
#include <vector>

using std::string;
using std::vector;

class Scenario
{
public:
	static Scenario *instance();
	
	const string GetName() { return m_name; }
	void SetName(const string name) { m_name = string(name, 0, 31); }
	
	const string GetVersion() { return m_version; }
	void SetVersion(const string version) { m_version = string(version, 0, 7); }

	const string GetID() { return m_id; }
	void SetID(const string id) { m_id = string(id, 0, 23); }
	
	bool IsCompatible(const string);
	void AddCompatible(const string);

	void SetAllowsClassicGameplay(bool allow) { m_allows_classic_gameplay = allow; }
	bool AllowsClassicGameplay() const { return m_allows_classic_gameplay; }
	
private:
	Scenario() : m_allows_classic_gameplay{false} { }
	
	string m_name;
	string m_version;
	string m_id;
	
	vector<string> m_compatibleVersions;

	bool m_allows_classic_gameplay;
};

class InfoTree;
void parse_mml_scenario(const InfoTree& root);
void reset_mml_scenario();

#endif
