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

#ifndef FRESHCITY_APPLICATION_GAMEMODE_MANAGER_CLASSES
#define FRESHCITY_APPLICATION_GAMEMODE_MANAGER_CLASSES

#include "application_gamemode_manager_base.h"
#include <boost/function.hpp>

// ProfileManager
#include "application_data_profile.h"

class ProfileManager : public ItemManager<ProfileManager, int, Profile> {
public:
	bool Add(int playerid);
};

// CommandManager
typedef void (*COMMAND_CALLBACK)(Profile& player, const char* cmdline);
typedef boost::function<void(Profile&, const char*)> CommandPtr;

struct CommandCallbackCell {
	CommandPtr ptr;
	int reqlevel;
	unsigned int flags;
	CommandCallbackCell(CommandPtr ptr, int reqlevel, unsigned int flags) : ptr(ptr), reqlevel(reqlevel), flags(flags) {}
};

class CommandManager : public ItemManager<CommandManager, std::string, CommandCallbackCell> {
public:
	bool Add(const std::string& cmd, COMMAND_CALLBACK function, int reqlevel, unsigned int flags);
	void Exec(int playerid, const std::string& cmd, const char* cmdline);
};

enum CommandRequirement {
	NEED_REGISTERED = 1,
	NEED_SIGNED_IN = 2,
	DONOT_REGISTERED = 4,
	DONOT_SIGNED_IN = 8,
	NO_REQUIREMENT = 16
};

// DialogManager
typedef void (*DIALOG_CALLBACK)(Profile& player, bool response, int listitem, const char* inputtext);
typedef boost::function<void(Profile&, bool, int, const char*)> DialogPtr;

class DialogManager : public ItemManager<DialogManager, int, DialogPtr> {
public:
	bool Add(int dialogid, DIALOG_CALLBACK function);
	void Exec(int playerid, bool response, int dialogid, int listitem, const char* inputtext);
};

// EffectiveItemManager
#include "application_gamemode_effectiveitem.h"

class EffectiveItemManager : public ItemManager<EffectiveItemManager, int, EffectiveItem> {
public:
	bool Add(MemberPtr& item);
	void Exec(int playerid, int itemid);
};

class PickupManager : public EffectiveItemManager {};

// ObjectManager
#include "application_gamemode_object.h"

class ObjectManager : public ItemManager<ObjectManager, int, DynamicObject> {
public:
	bool Add(MemberPtr& item) {
		return ItemManager::Add(item->GetID(), item);
	}
};

// TeamManager
#include "application_data_team.h"
#include "basic_algorithm_identifier.h"

class TeamManager : public ItemManager<TeamManager, std::string, Team> {
private:
	IdentifierAllocator _idgen;

protected:
	TeamManager();

public:
	friend class ItemManager<TeamManager, std::string, Team>;
	bool Add(const mongo::OID& leader, const std::string& name, int color);
	bool Remove(const mongo::OID& teamid);
	void LoadAllFromDatabase();
	Team& operator[](const mongo::OID& teamid);
};

// GangZoneManager
#include "application_data_gangzone.h"

class GangZoneManager : public ItemManager<GangZoneManager, int, GangZoneItem> {
public:
	bool Add(MemberPtr& item);
	void LoadAllFromDatabase();
	int GetPointInWhichZone(Coordinate3D& point) const;
};

// DynamicAreaManager
#include "application_gamemode_dynamicarea.h"

class DynamicAreaManager : public ItemManager<DynamicAreaManager, int, DynamicArea> {
public:
	bool Add(MemberPtr& item);
};

#endif
