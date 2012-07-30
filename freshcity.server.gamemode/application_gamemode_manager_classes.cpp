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
	if(iter == _members.end()) throw std::runtime_error("Unregistered command.");
	Profile& player = ProfileManager::GetInstance()[playerid];
	unsigned int flags = iter->second->flags;
	if(flags != 0) {
		if(MATCHREQ(NEED_REGISTERED) && ProfileManager::GetInstance()[playerid].IsEmpty())
			throw std::runtime_error("此命令仅限已注册玩家使用");
		if(MATCHREQ(NEED_SIGNED_IN) && !ProfileManager::GetInstance()[playerid].IsSignedIn())
			throw std::runtime_error("此命令仅限已登录玩家使用");
		if(MATCHREQ(DONOT_REGISTERED) && !ProfileManager::GetInstance()[playerid].IsEmpty())
			throw std::runtime_error("此命令仅限未注册玩家使用");
		if(MATCHREQ(DONOT_SIGNED_IN) && ProfileManager::GetInstance()[playerid].IsSignedIn())
			throw std::runtime_error("此命令仅限未登录玩家使用");
	}
	int levelreq = iter->second->reqlevel;
	if(levelreq > player.GetAdminLevel())
		throw std::runtime_error("您至少需要 Level " + boost::lexical_cast<std::string>(levelreq) + " 来执行此命令");
	iter->second->ptr(player, cmdline);
}

#undef MATCHREQ

// DialogManager
bool DialogManager::Add(int dialogid, const DialogCell& cell) {
	return ItemManager::Add(dialogid, MemberPtr(new DialogCell(cell)));
}

void DialogManager::Show(int dialogid, const std::string& content, int playerid, bool showforall) {
	MemberMap::const_iterator dlg(_members.find(dialogid));
	if(dlg == _members.end()) throw std::runtime_error("Unregistered dialog.");
	DialogCell& dlginfo(*dlg->second.get());
	if(showforall) {
		MANAGER_FOREACH(ProfileManager) ShowPlayerDialog(iter->first, dialogid, dlginfo.style,
		dlginfo.caption.c_str(), content.c_str(), dlginfo.btnOK.c_str(), dlginfo.btnCancel.c_str());
	} else {
		ShowPlayerDialog(playerid, dialogid, dlginfo.style,
		dlginfo.caption.c_str(), content.c_str(), dlginfo.btnOK.c_str(), dlginfo.btnCancel.c_str());
	}
}

void DialogManager::Exec(int playerid, bool response, int dialogid, int listitem, const char* inputtext) {
	MemberMap::const_iterator iter(_members.find(dialogid));
	if(iter == _members.end()) throw std::runtime_error("Unregistered dialog.");
	Profile& player = ProfileManager::GetInstance()[playerid];
	iter->second->callback.operator()(player, response, listitem, inputtext);
}

// EffectiveItemManager
bool EffectiveItemManager::Add(const MemberPtr& item) {
	return ItemManager::Add(item->GetID(), item);
}

void EffectiveItemManager::Exec(int playerid, int itemid) {
	MemberMap::const_iterator iter(_members.find(itemid));
	if(iter == _members.end()) throw std::runtime_error("Invalid object id.");
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
bool GangZoneManager::Add(const MemberPtr& item) {
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
bool DynamicAreaManager::Add(const MemberPtr& item) {
	return ItemManager::Add(item->GetID(), item);
}
