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
#include "application_gamemode_colordefinitions.h"
#include "application_data_waypoint.h"
#include <sampgdk/a_players.h>
#include <sampgdk/a_vehicles.h>
#include "application_dependency_streamer.h"
#include "application_data_object.h"
#include "application_algorithm_position.h"
#include "application_gamemode_sysmsgqueue.h"

class _CmdRegister {
public:
	_CmdRegister(const std::string& cmd, COMMAND_CALLBACK function, int reqlevel, unsigned int flags) {
		CommandManager::GetInstance().Add(cmd, function, reqlevel, flags);
	}
};

#define CMD(callback, cmd, levelreq, flags) \
	void Cmd##callback(Profile& player, const char* cmdline);\
	_CmdRegister __CmdReg##callback(cmd, Cmd##callback, levelreq, flags);\
	void Cmd##callback(Profile& player, const char* cmdline)\

CMD(SaveData, "sync", 0, NEED_SIGNED_IN) {
	player.Sync();
	player.SendChatMessage(COLOR_SUCC, "数据已保存");
}

CMD(SetSkin, "setskin", 0, NEED_SIGNED_IN) {
	int skinid(-1);
	if(sscanf(cmdline, "%d", &skinid) == 0 || skinid == -1) throw std::runtime_error("用法: /setskin <皮肤ID>");
	player.SetSkin(skinid);
	player.SendChatMessage(COLOR_SUCC, "皮肤已更改");
}

CMD(GiveWeapon, "giveweapon", 1, NEED_SIGNED_IN) {
	int target(-1), weapon(-1), ammo(-1);
	if(sscanf(cmdline, "%d%d%d", &target, &weapon, &ammo) == 0
		|| !(target != -1 && weapon != -1 && ammo != -1))
		throw std::runtime_error("用法: /giveweapon <玩家ID> <武器ID> <弹药量>");
	GivePlayerWeapon(target, weapon, ammo);
}

CMD(GetVehicle, "v", 0, NO_REQUIREMENT) {
	int mid(-1);
	sscanf(cmdline, "%d", &mid);
	Coordinate3D playerpos = player.GetPos();
	int vid = CreateVehicle(mid, (float)playerpos.x, (float)playerpos.y, (float)playerpos.z + 0.5f,
		player.GetFacingAngle(), 1, 1, -1);
	LinkVehicleToInterior(vid, player.GetInterior());
	SetVehicleVirtualWorld(vid, player.GetVirtualWorld());
	PutPlayerInVehicle(player.GetId(), vid, 0);
}

CMD(CreateWaypoint, "ctp", 0, NEED_SIGNED_IN) {
	if(cmdline[0] == 0)	throw std::runtime_error("用法: /ctp <传送点名称>");
	Waypoint create(player.GetDetailedPos());
	create.Create(cmdline, player.GetUniqueID());
	player.SendChatMessage(COLOR_SUCC, "已创建传送点 " + std::string(cmdline));
}

CMD(UseWaypoint, "tp", 0, NO_REQUIREMENT) {
	if(cmdline[0] == 0)	throw std::runtime_error("用法: /tp <传送点名称>");
	Waypoint point(cmdline);
	point.PerformTeleport(player.GetId());
	player.SendChatMessage(COLOR_SUCC, "已传送到 " + std::string(cmdline));
}

CMD(GoToPlayer, "goto", 1, NEED_SIGNED_IN) {
	int targetid(-1);
	sscanf(cmdline, "%d", &targetid);
	if(!IsPlayerConnected(targetid)) throw std::runtime_error("用法: /get <玩家ID>");
	Waypoint point(ProfileManager::GetInstance()[targetid].GetDetailedPos());
	point.PerformTeleport(player.GetId());
}

CMD(GetPlayer, "get", 1, NEED_SIGNED_IN) {
	int targetid(-1);
	sscanf(cmdline, "%d", &targetid);
	if(!IsPlayerConnected(targetid)) throw std::runtime_error("用法: /get <玩家ID>");
	Waypoint point(player.GetDetailedPos());
	point.PerformTeleport(targetid);
}
