/*
 * Copyright 2012 TennenColl
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FRESHCITY_APPLICATION_GAMEMODE_MANAGER_TEAM
#define FRESHCITY_APPLICATION_GAMEMODE_MANAGER_TEAM

#include "application_gamemode_manager_base.h"
#include "application_data_team.h"

class TeamManager : public BaseManager<TeamManager, std::string, Team> {
private:
	int _teamidcounter;
	typedef boost::unordered_map<int, std::string> IDNameMap;
	IDNameMap _idtoname;

protected:
	TeamManager();

public:
	friend class BaseManager<TeamManager, std::string, Team>;
	bool Add(const std::string& teamname);
	bool Remove(const std::string& teamname);
	std::string GetNameByID(int teamid) const;
};

#endif
