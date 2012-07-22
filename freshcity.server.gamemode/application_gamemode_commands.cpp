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
#include "application_gamemode_manager_command.h"
#include "application_gamemode_manager_profile.h"
#include "application_gamemode_colordefinitions.h"
#include "application_data_waypoint.h"
#include <sampgdk/a_players.h>
#include <sampgdk/a_vehicles.h>
#include "application_gamemode_manager_team.h"
#include "application_dependency_streamer.h"
#include "application_gamemode_manager_object.h"
#include "application_data_object.h"
#include "application_algorithm_position.h"

#define CMD(x) void Cmd##x(Profile& player, const char* cmdline)

CMD(SaveData) {
	player.Sync();
	player.SendChatMessage(COLOR_SUCC, "数据已保存");
}

CMD(SetSkin) {
	int skinid(-1);
	if(sscanf(cmdline, "%d", &skinid) == 0 || skinid == -1) throw std::runtime_error("用法: /setskin <皮肤ID>");
	player.SetSkin(skinid);
	player.SendChatMessage(COLOR_SUCC, "皮肤已更改");
}

CMD(GiveWeapon) {
	int target(-1), weapon(-1), ammo(-1);
	if(sscanf(cmdline, "%d%d%d", &target, &weapon, &ammo) == 0
		|| !(target != -1 && weapon != -1 && ammo != -1))
		throw std::runtime_error("用法: /giveweapon <玩家ID> <武器ID> <弹药量>");
	GivePlayerWeapon(target, weapon, ammo);
}

CMD(GetVehicle) {
	int mid(-1);
	sscanf(cmdline, "%d", &mid);
	Coordinate3D playerpos = player.GetPos();
	int vid = CreateVehicle(mid, (float)playerpos.x, (float)playerpos.y, (float)playerpos.z + 0.5f, 0, 1, 1, -1);
	LinkVehicleToInterior(vid, player.GetInterior());
	SetVehicleVirtualWorld(vid, player.GetVirtualWorld());
	PutPlayerInVehicle(player.GetId(), vid, 0);
}

CMD(CreateWaypoint) {
	if(cmdline[0] == 0)	throw std::runtime_error("用法: /ctp <传送点名称>");
	Waypoint create(player.GetDetailedPos());
	create.Create(cmdline, player.GetUniqueID());
	player.SendChatMessage(COLOR_SUCC, "已创建传送点 " + std::string(cmdline));
}

CMD(UseWaypoint) {
	if(cmdline[0] == 0)	throw std::runtime_error("用法: /tp <传送点名称>");
	Waypoint point(cmdline);
	point.PerformTeleport(player.GetId());
	player.SendChatMessage(COLOR_SUCC, "已传送到 " + std::string(cmdline));
}

CMD(TeamJoin) {
	TeamManager::GetInstance()[cmdline].Join(player);
}

CMD(TeamQuit) {
	TeamManager::GetInstance()[TeamManager::GetInstance().GetNameByID(player.GetTeamFixed())].Quit(player);
}

CMD(GoToPlayer) {
	int targetid(-1);
	sscanf(cmdline, "%d", &targetid);
	if(!IsPlayerConnected(targetid)) throw std::runtime_error("用法: /get <玩家ID>");
	Waypoint point(ProfileManager::GetInstance()[targetid].GetDetailedPos());
	point.PerformTeleport(player.GetId());
}

CMD(GetPlayer) {
	int targetid(-1);
	sscanf(cmdline, "%d", &targetid);
	if(!IsPlayerConnected(targetid)) throw std::runtime_error("用法: /get <玩家ID>");
	Waypoint point(player.GetDetailedPos());
	point.PerformTeleport(targetid);
}

CMD(CreateDynObject) {
	int modelid(-1);
	sscanf(cmdline, "%d", &modelid);
	Coordinate3D pos = GenerateDirectionalPoint(player, 10.0f);
	ObjectManager::MemberPtr ptr(new DynamicObject(
		modelid, pos.x, pos.y, pos.z - 1, 0.0f, 0.0f, 0.0f));
	ObjectManager::GetInstance().Add(ptr);
}

CMD(IteratorTest) {
	TeamManager::MemberMap::iterator iter = TeamManager::GetInstance().GetIterator(), end;
	for(;iter != end; iter++) {
		player.SendChatMessage(COLOR_YELLOW, iter->first);
	}
}

#define REGCMD(x, y, z, t) CmdMgr.Add(x, y, z, t)

bool RegisterPlayerCmds() {
	CommandManager& CmdMgr = CommandManager::GetInstance();
	REGCMD("sync",				CmdSaveData,			0, NEED_SIGNED_IN);
	REGCMD("setskin",			CmdSetSkin,				1, NEED_SIGNED_IN);
	REGCMD("giveweapon",		CmdGiveWeapon,			1, NEED_SIGNED_IN);
	REGCMD("v",					CmdGetVehicle,			0, NO_REQUIREMENT);
	REGCMD("ctp",				CmdCreateWaypoint,		0, NEED_SIGNED_IN);
	REGCMD("tp",				CmdUseWaypoint,			0, NO_REQUIREMENT);
	REGCMD("teamjoin",			CmdTeamJoin,			5, NEED_SIGNED_IN);
	REGCMD("teamquit",			CmdTeamQuit,			5, NEED_SIGNED_IN);
	REGCMD("goto",				CmdGoToPlayer,			1, NEED_SIGNED_IN);
	REGCMD("get",				CmdGetPlayer,			1, NEED_SIGNED_IN);
	REGCMD("object",			CmdCreateDynObject,		0, NO_REQUIREMENT);
	REGCMD("iterator",			CmdIteratorTest,		0, NO_REQUIREMENT);
	return true;
}

#undef REGCMD

void* PlayerCmdInit((void*)RegisterPlayerCmds());
