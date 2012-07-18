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

#define CMD(x) void Cmd##x(Profile& player, const char* cmdline)

CMD(Register) {
	if(cmdline[0] == 0) throw std::runtime_error("密码不能为空.");
	try {
		player.Create(player.GetName(), cmdline);
		player.SendChatMessage(COLOR_SUCC, "注册成功. 登录名: " + player.GetName() + ", 密码(引号内): \"" + cmdline + "\".");
		player.SetSignedIn(true);
	} catch(std::runtime_error) {
		throw;
	} catch(...) {
		throw std::runtime_error("注册失败.");
	}
}

CMD(Login) {
	if(!player.AuthPassword(cmdline)) throw std::runtime_error("密码错误.");
	player.SetSignedIn(true);
	player.ApplyDataToPlayer();
	player.SendChatMessage(COLOR_SUCC, "登录成功.");
}

CMD(LogOut) {
	player.SetSignedIn(false);
	player.SendChatMessage(COLOR_SUCC, "您已登出.");
}

CMD(SaveData) {
	player.Sync();
	player.SendChatMessage(COLOR_SUCC, "数据已保存.");
}

CMD(SetSkin) {
	int skinid(-1);
	if(sscanf(cmdline, "%d", &skinid) == 0 || skinid == -1) throw std::runtime_error("用法: /setskin <皮肤ID>");
	player.SetSkin(skinid);
	player.SendChatMessage(COLOR_SUCC, "皮肤已更改.");
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
	create.Create(cmdline, player.GetID());
	player.SendChatMessage(COLOR_SUCC, "已创建传送点 \"" + std::string(cmdline) + "\" .");
}

CMD(UseWaypoint) {
	if(cmdline[0] == 0)	throw std::runtime_error("用法: /tp <传送点名称>");
	Waypoint point(cmdline);
	point.PerformTeleport(player.GetId());
	player.SendChatMessage(COLOR_SUCC, "已传送到 \"" + std::string(cmdline) + "\" .");
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

CMD(CreatePickupHere) {
	int pickupid(-1);
	sscanf(cmdline, "%d", &pickupid);
	Coordinate3D playerpos = player.GetPos();
	CreateDynamicPickup(pickupid, 1, playerpos.x, playerpos.y, playerpos.z);
}

#define REGCMD(x, y, z, t) CmdMgr.Add(x, y, z, t)

bool RegisterPlayerCmds() {
	CommandManager& CmdMgr = CommandManager::GetInstance();
	REGCMD("register",			CmdRegister,			0, DONOT_REGISTERED);
	REGCMD("login",				CmdLogin,				0, NEED_REGISTERED | DONOT_SIGNED_IN);
	REGCMD("logout",			CmdLogOut,				0, NEED_SIGNED_IN);
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
	REGCMD("pickup",			CmdCreatePickupHere,	0, NO_REQUIREMENT);
	return true;
}

#undef REGCMD

void* PlayerCmdInit((void*)RegisterPlayerCmds());
