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
#include "application_config.h"
#include "basic_algorithm_gbkencoder.h"
#include "application_data_team.h"
#include <sampgdk/a_players.h>
#include "application_gamemode_colordefinitions.h"
#include "basic_algorithm_random.h"

Team::Team(const std::string& name, int teamid) : SaveableItem(CONFIG_STRING("Database.team")),
	_name(name), _score(0), _ingameteamid(teamid), _color(RandomRGBAColor()), _level(0) {}

void Team::Join(Profile& player) {
	if(IsMember(player.GetId()))
		throw std::runtime_error("您已经是该团队的成员了");
	if(player.GetTeamFixed() != NO_TEAM)
		throw std::runtime_error("您已加入过别的团队了 请执行 /teamquit 退出先前加入的团队");
	player.SetTeamFixed(_ingameteamid);
	player.SendChatMessage(COLOR_SUCC, "您已加入团队 " + _name);
	_onlineplayers.insert(std::make_pair(player.GetId(), player.GetUniqueID()));
}

void Team::Quit(Profile& player) {
	TeamMemberMap::iterator iter = _onlineplayers.find(player.GetId());
	if(iter == _onlineplayers.end())
		throw std::runtime_error("您并未加入此团队");
	player.SetTeamFixed(NO_TEAM);
	player.SendChatMessage(COLOR_SUCC, "您已退出团队 " + _name);
	_onlineplayers.erase(iter);
}

void Team::SetName(const std::string& name) {
	_name = name;
}

std::string Team::GetName() const {
	return _name;
}

void Team::SetColor(int color) {
	_color = color;
}

int Team::GetColor() const {
	return _color;
}

void Team::SetLevel(int level) {
	_level = level;
}

int Team::GetLevel() const {
	return _level;
}

bool Team::IsMember(int playerid) const {
	return _onlineplayers.find(playerid) != _onlineplayers.end();
}

int Team::GetScore() {
	return _score;
}

void Team::IncreaseScore(int amount) {
	_score += amount;
}

int Team::GetIngameID() {
	return _ingameteamid;
}

bool Team::HasOnlineMember() {
	return !_onlineplayers.empty();
}

Team::TeamMemberMap::iterator Team::GetMemberIterator() {
	return TeamMemberMap::iterator(_onlineplayers.begin());
}
