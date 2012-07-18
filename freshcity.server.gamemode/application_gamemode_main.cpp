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
#include <sampgdk/a_players.h>
#include <sampgdk/a_samp.h>
#include <sampgdk/a_vehicles.h>
#include <sampgdk/plugin.h>
#include "basic_debug_logging.h"
#include "application_gamemode_manager_profile.h"
#include "application_gamemode_manager_command.h"
#include "application_gamemode_colordefinitions.h"
#include <boost/algorithm/string.hpp>
#include "basic_algorithm_random.h"
#include "application_gamemode_manager_team.h"
#include "application_gamemode_manager_dialog.h"
#include "application_gamemode_dialogdefinitions.h"

ProfileManager& ProfileMgr(ProfileManager::GetInstance());
TeamManager& TeamMgr(TeamManager::GetInstance());

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() {
	return SUPPORTS_VERSION | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppPluginData) {
	sampgdk_initialize_plugin(ppPluginData);
	return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload() {
	return;
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick() {
	sampgdk_process_timers();
}

PLUGIN_EXPORT bool PLUGIN_CALL OnGameModeInit() {
	SetGameModeText("Freshcity");
	for(int i = 1; i < 299; i++) 
		AddPlayerClass(i, 1497.07f, -689.485f, 94.956f, 180.86f, 16, 3, 27, 100, 31, 100);
	TeamMgr.Add("Cops");
	TeamMgr["Cops"].SetColor(COLOR_BLUE);
	TeamMgr.Add("Criminals");
	TeamMgr["Criminals"].SetColor(COLOR_GREEN);
	LOG_INFO("Freshcity Gamemode 已载入");
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerConnect(int playerid) {
	try {
		SendClientMessage(playerid, COLOR_INFO, "欢迎来到 TennenColl 的开发服务器");
		try {
			ProfileMgr.Add(playerid);
			ProfileMgr[playerid].SetSignedIn(false);
			Profile& player = ProfileMgr[playerid];
			if(!player.IsExistInDatabase()) {
				if(player.IsBannedForGame()) {
					player.SendChatMessage(COLOR_ERROR, "你已经被服务器封禁.");
					player.KickNow();
				} else {
					//player.SetColor(RandomRGBAColor());
					player.SendChatMessage(COLOR_INFO, "你还没有注册, 请 /register <密码> 来创建新用户.");
				}
			} else {
				player.SendChatMessage(COLOR_WARN, "欢迎回来, " + player.GetName() + " . 请执行 /login <密码> 以登录.");
			}
			player.SetTeamFixed(NO_TEAM);
			SendClientMessageToAll(COLOR_INFO, std::string(player.GetName() + " 进入服务器.").c_str());
			SendDeathMessage(INVALID_PLAYER_ID, playerid, 200);
			player.SendChatMessage(COLOR_INFO, "/teamjoin Cops 加入警察");
			player.SendChatMessage(COLOR_INFO, "/teamjoin Criminals 加入罪犯");
			player.SendChatMessage(COLOR_WARN, "请注意大小写");
		} catch(std::runtime_error& e) {
			LOG_ERROR(e.what());
			throw;
		} catch(mongo::UserException& e) {
			LOG_ERROR(e.what());
			throw;
		} catch(...) {
			throw;
		}
	} catch(...) {
		SendClientMessage(playerid, COLOR_ERROR, "初始化玩家数据时发生错误, 请联系服务器管理员.");
		return false;
	}
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDisconnect(int playerid, int reason) {
	try {
		Profile& player =  ProfileMgr[playerid];
		if(reason != 0 /* timeout */ && player.IsSignedIn())
			player.Sync();
		int playerteamid = player.GetTeamFixed();
		if(playerteamid != NO_TEAM)
			TeamMgr[TeamMgr.GetNameByID(playerteamid)].Quit(ProfileMgr[playerid]);
		ProfileMgr.Remove(playerid);
		SendClientMessageToAll(COLOR_INFO, std::string(GetPlayerName(playerid) + " 离开服务器.").c_str());
		SendDeathMessage(INVALID_PLAYER_ID, playerid, 201);
	} catch(...) {
		return false;
	}
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerRequestClass(int playerid, int classid) {
	SetPlayerPos(playerid, 1497.07f, -689.485f, 94.956f);
	SetPlayerFacingAngle(playerid, 180.86f);
	SetPlayerCameraPos(playerid, 1497.81f, -707.83f, 99.69f);
	SetPlayerCameraLookAt(playerid, 1493.39f, -686.97f, 98.35f, CAMERA_MOVE);
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerCommandText(int playerid, const char *cmdtext) {
	LOG_DEBUG(cmdtext);
	char cmdname[128] = {0}, cmdline[128] = {0};
	sscanf(cmdtext, "%s%*[ ]%[^\0]", cmdname, cmdline);
	try {
		CommandManager::GetInstance().Exec(playerid, boost::to_lower_copy(std::string(&cmdname[1])), cmdline);
		return true;
	} catch(std::runtime_error& e) {
		SendClientMessage(playerid, COLOR_ERROR, e.what());
		return true;
	} catch(...) {
		SendClientMessage(playerid, COLOR_ERROR, "处理命令时发生未知错误, 请联系服务器管理员.");
		return true;
	}
	return false;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys) {
	if((newkeys & KEY_FIRE) == KEY_FIRE || (newkeys & KEY_ACTION) == KEY_ACTION)
		if(IsPlayerInAnyVehicle(playerid))
			AddVehicleComponent(GetPlayerVehicleID(playerid), 1010);
	return true;	
}

PLUGIN_EXPORT bool PLUGIN_CALL OnDialogResponse(int playerid, int dialogid, int response, int listitem, const char *inputtext) {
	LOG_DEBUG("DialogID: " << dialogid << " Response: " << response << " Listitem: " << listitem << " Input: " << inputtext);
	try {
		DialogManager::GetInstance().Exec(playerid, response == 1, dialogid, listitem, inputtext);
	} catch(std::runtime_error& e) {
		SendClientMessage(playerid, COLOR_ERROR, e.what());
		return true;
	} catch(...) {
		SendClientMessage(playerid, COLOR_ERROR, "处理对话框时发生未知错误, 请联系服务器管理员.");
		return true;
	}
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerClickMap(int playerid, float x, float y, float z) {
	SetPlayerPos(playerid, x, y, z);
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDeath(int playerid, int killerid, int reason) {
	LOG_DEBUG("Killer: " << killerid << " Victim: " << playerid << " Reason: " << reason);
	GivePlayerMoney(playerid, -500);
	if(killerid != INVALID_PLAYER_ID) {
		SetPlayerScore(killerid, GetPlayerScore(killerid) + 1);
		GivePlayerMoney(killerid, 1000);
	}
	SendDeathMessage(killerid, playerid, reason);
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerSpawn(int playerid) {
	ShowPlayerDialog(playerid, DIALOG_TEAM_SELECT, DIALOG_STYLE_LIST, "请选择您的阵营",  "Cops\nCriminals", "确定", "");
	return true;
}
