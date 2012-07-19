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

#include "application_database.h"
#include "application_gamemode_manager_team.h"

TeamManager::TeamManager() : _teamidcounter(0) {}

bool TeamManager::Add(const std::string& teamname) {
	MemberPtr ptr(new Team(teamname, ++_teamidcounter));
	if(ItemManager::Add(teamname, ptr)) {
		_idtoname.insert(std::make_pair(_teamidcounter, teamname));
		return true;
	} else return false;
}

bool TeamManager::Remove(const std::string& teamname) {
	int tid = ItemManager::Get(teamname).GetIngameID();
	if(ItemManager::Remove(teamname)) {
		_idtoname.erase(_idtoname.find(tid));
		return true;
	} return false;
}

std::string TeamManager::GetNameByID(int teamid) const {
	IDNameMap::const_iterator iter = _idtoname.find(teamid);
	if(iter == _idtoname.end())
		throw std::runtime_error("无法获得指定团队的ID");
	return iter->second;
}
