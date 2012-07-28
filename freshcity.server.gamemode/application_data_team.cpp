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

void Team::__LoadData() {
	_name	= UTF8ToGBK(_rawdata["name"].String());
	_color	= (int)_rawdata["color"].Number();
	_leader	= _rawdata["leader"].OID();
}

Team::Team(const mongo::OID& uniqueid, int teamid)
	: SaveableItem(CONFIG_STRING("Database.team"), uniqueid), _ingameteamid(teamid) {
		__LoadData();
}

Team::Team(const mongo::BSONObj& data, int teamid)
	: SaveableItem(CONFIG_STRING("Database.team")), _ingameteamid(teamid) {
		InitData(data);
		__LoadData();
}

Team::Team(const mongo::OID& leader, const std::string& name, int color, int teamid)
	: SaveableItem(CONFIG_STRING("Database.team")), _name(name), _leader(leader),
	_ingameteamid(teamid), _color(color) {}

void Team::Join(Profile& player) {
	if(InGame(player.GetId()))
		throw std::runtime_error("您已经是该团队的成员了");
	if(player.GetTeamId().isSet())
		throw std::runtime_error("您已加入过别的团队了 请执行 /teamquit 退出先前加入的团队");
	player.SetTeam(_ingameteamid);
	player.SetTeamId(_uniqueid);
	player.SendChatMessage(COLOR_SUCC, "您已加入团队 " + _name);
	_onlineplayers.insert(std::make_pair(player.GetId(), player.GetUniqueID()));
}

void Team::Quit(Profile& player) {
	TeamMemberMap::iterator iter = _onlineplayers.find(player.GetId());
	if(iter == _onlineplayers.end())
		throw std::runtime_error("您并未加入此团队");
	player.SetTeam(NO_TEAM);
	player.SetTeamId(mongo::OID());
	player.SendChatMessage(COLOR_SUCC, "您已退出团队 " + _name);
	_onlineplayers.erase(iter);
}

void Team::SetName(const std::string& name) {
	_name = name;
	if(IsEmpty()) return;
	Update(BSON("$set" << BSON("name" << GBKToUTF8(name))), false);
}

std::string Team::GetName() const {
	return _name;
}

void Team::SetColor(int color) {
	_color = color;
	if(IsEmpty()) return;
	Update(BSON("$set" << BSON("color" << color)), false);
}

int Team::GetColor() const {
	return _color;
}

bool Team::InGame(int playerid) const {
	return _onlineplayers.find(playerid) != _onlineplayers.end();
}

int Team::GetIngameID() const {
	return _ingameteamid;
}

bool Team::HasOnlineMember() const {
	return !_onlineplayers.empty();
}

Team::TeamMemberMap::iterator Team::GetMemberIterator() {
	return TeamMemberMap::iterator(_onlineplayers.begin());
}

void Team::Create() {
	mongo::BSONObj submit(BSON(mongo::GENOID <<
		"name"		<< _name <<
		"leader"	<< _leader <<
		"color"		<< _color
		));
	SaveableItem::Create(submit, false);
}

mongo::OID Team::GetLeader() {
	return _leader;
}
