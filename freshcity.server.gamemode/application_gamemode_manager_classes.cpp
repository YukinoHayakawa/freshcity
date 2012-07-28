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
#include "application_gamemode_manager_classes.h"
#include "application_config.h"

#define FETCH_ALL_FROM_DATABASE(col) std::auto_ptr<mongo::DBClientCursor> _cursor = \
	GetDB().query(CONFIG_STRING(col), mongo::BSONObj());\
	while(_cursor->more())

// ProfileManager
bool ProfileManager::Add(int playerid) {
	return ItemManager::Add(playerid, MemberPtr(new Profile(playerid, GetPlayerName(playerid))));
}

// CommandManager
bool CommandManager::Add(const std::string& cmd, COMMAND_CALLBACK function, int reqlevel, unsigned int flags) {
	return ItemManager::Add(cmd, MemberPtr(new CommandCallbackCell(CommandPtr(function), reqlevel, flags)));
}

#define MATCHREQ(req) ((flags & req) == req)

void CommandManager::Exec(int playerid, const std::string& cmd, const char* cmdline) {
	MemberMap::const_iterator iter(_members.find(cmd));
	if(iter == _members.end()) throw std::runtime_error("不存在的命令");
	Profile& player = ProfileManager::GetInstance()[playerid];
	unsigned int flags = iter->second->flags;
	if(!MATCHREQ(NO_REQUIREMENT)) {
		if(MATCHREQ(NEED_REGISTERED) && ProfileManager::GetInstance()[playerid].IsEmpty())
			throw std::runtime_error("此命令仅限已注册玩家使用");
		if(MATCHREQ(NEED_SIGNED_IN) && !ProfileManager::GetInstance()[playerid].IsSignedIn())
			throw std::runtime_error("此命令仅限已登录玩家使用");
		if(MATCHREQ(DONOT_REGISTERED) && !ProfileManager::GetInstance()[playerid].IsEmpty())
			throw std::runtime_error("此命令仅限未注册玩家使用");
		if(MATCHREQ(DONOT_SIGNED_IN) && ProfileManager::GetInstance()[playerid].IsSignedIn())
			throw std::runtime_error("此命令仅限未登录玩家使用");
	}
	if(iter->second->reqlevel > player.GetAdminLevel())
		throw std::runtime_error("您没有足够管理权限来执行此命令");
	iter->second->ptr(player, cmdline);
}

#undef MATCHREQ

// DialogManager
bool DialogManager::Add(int dialogid, DIALOG_CALLBACK function) {
	return ItemManager::Add(dialogid, MemberPtr(new DialogPtr(function)));
}

void DialogManager::Exec(int playerid, bool response, int dialogid, int listitem, const char* inputtext) {
	MemberMap::const_iterator iter(_members.find(dialogid));
	if(iter == _members.end()) throw std::runtime_error("未注册回调的对话框");
	Profile& player = ProfileManager::GetInstance()[playerid];
	iter->second->operator()(player, response, listitem, inputtext);
}

// EffectiveItemManager
bool EffectiveItemManager::Add(MemberPtr& item) {
	return ItemManager::Add(item->GetID(), item);
}

void EffectiveItemManager::Exec(int playerid, int itemid) {
	MemberMap::const_iterator iter(_members.find(itemid));
	if(iter == _members.end()) throw std::runtime_error("无效对象ID");
	Profile& player = ProfileManager::GetInstance()[playerid];
	iter->second->Effect(player);
	if(iter->second->IsDisposable()) _members.erase(iter);
}

// TeamManager
TeamManager::TeamManager() : _idgen(255) {}

bool TeamManager::Add(const mongo::OID& leader, const std::string& name, int color) {
	int id = _idgen.Get();
	MemberPtr item(new Team(leader, name, color, id));
	item->Create();
	return ItemManager::Add(item->GetUniqueID().str(), item);
}

bool TeamManager::Remove(const mongo::OID& teamid) {
	MemberMap::iterator iter(_members.find(teamid.str()));
	int id = iter->second->GetIngameID();
	if(ItemManager::Remove(iter->second->GetUniqueID().str())) {
		_idgen.Return(id);
		return true;
	} return false;
}

void TeamManager::LoadAllFromDatabase() {
	MANAGER_FOREACH(TeamManager) _idgen.Return(iter->second->GetIngameID());
	_members.clear();
	FETCH_ALL_FROM_DATABASE("Database.team") {
		MemberPtr item(new Team(_cursor->next(), _idgen.Get()));
		ItemManager::Add(item->GetUniqueID().str(), item);
	}
}

Team& TeamManager::operator[](const mongo::OID& teamid) {
	return Get(teamid.str());
}

// GangZoneManager
bool GangZoneManager::Add(MemberPtr& item) {
	return ItemManager::Add(item->Get().GetId(), item);
}

void GangZoneManager::LoadAllFromDatabase() {
	_members.clear();
	FETCH_ALL_FROM_DATABASE("Database.gangzone") {
		MemberPtr _item(new GangZoneItem(_cursor->next()));
		ItemManager::Add(_item->Get().GetId(), _item);
	}
}

int GangZoneManager::GetPointInWhichZone(Coordinate3D& point) const {
	for(MemberMap::const_iterator iter = _members.begin(); iter != _members.end(); ++iter) {
		if(DynamicAreaManager::GetInstance()[iter->second->GetAreaID()].IsPointIn(point.x, point.y, point.z))
			return iter->first;
	}
	return -1;
}

// DynamicAreaManager
bool DynamicAreaManager::Add(MemberPtr& item) {
	return ItemManager::Add(item->GetID(), item);
}
