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
#include "application_dependency_streamer.h"
#include "application_data_waypoint.h"
#include "application_gamemode_manager_effectiveitem.h"
#include "application_gamemode_manager_effectiveitem.h"
#include "application_data_pickup_classes.h"
#include "application_algorithm_position.h"
#include "application_config.h"
#include <boost/random.hpp>
#include "application_gamemode_role_classes.h"

ProfileManager& ProfileMgr(ProfileManager::GetInstance());
TeamManager& TeamMgr(TeamManager::GetInstance());

#define STREAMER_CALLBACK __declspec(dllexport)

STREAMER_CALLBACK void OnPlayerEnterDynamicArea(int playerid, int areaid);
STREAMER_CALLBACK void OnPlayerLeaveDynamicArea(int playerid, int areaid);
STREAMER_CALLBACK void OnDynamicObjectMoved(int objectid);

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() {
	return SUPPORTS_VERSION | SUPPORTS_PROCESS_TICK;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppPluginData) {
	sampgdk_initialize_plugin(ppPluginData);
	SetOnPlayerEnterDynamicAreaCallback(OnPlayerEnterDynamicArea);
	SetOnPlayerLeaveDynamicAreaCallback(OnPlayerLeaveDynamicArea);
	SetOnDynamicObjectMovedCallback(OnDynamicObjectMoved);
	return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload() {
	return;
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick() {
	sampgdk_process_timers();
	Streamer_ProcessTick();
}

PLUGIN_EXPORT bool PLUGIN_CALL OnGameModeInit() {
	SetGameModeText("Freshcity");
	for(int i = 1; i < 299; i++) 
		AddPlayerClass(i, 1497.07f, -689.485f, 94.956f, 180.86f, 0, 0, 0, 0, 0, 0);
	TeamMgr.Add("Cops");
	TeamMgr["Cops"].SetColor(COLOR_BLUE);
	TeamMgr.Add("Criminals");
	TeamMgr["Criminals"].SetColor(COLOR_GREEN);
	LOG_INFO("Freshcity Gamemode 已载入");
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerConnect(int playerid) {
	try {
		Streamer_OnPlayerConnect(playerid);
		SendClientMessage(playerid, COLOR_INFO, "欢迎来到 TennenColl 的开发服务器");
		try {
			ProfileMgr.Add(playerid);
			ProfileMgr[playerid].SetSignedIn(false);
			Profile& player = ProfileMgr[playerid];
			if(!player.IsExistInDatabase()) {
				player.SetColor(RandomRGBAColor());
				ShowPlayerDialog(playerid, DIALOG_PROFILE_REGISTER, DIALOG_STYLE_INPUT, "注册", "请输入您的密码:", "注册", "");
			} else {
				if(player.IsBannedForGame()) {
					player.SendChatMessage(COLOR_ERROR, "你已经被服务器封禁");
					player.KickNow();
				} else
					ShowPlayerDialog(playerid, DIALOG_PROFILE_LOGIN, DIALOG_STYLE_INPUT, "登录", "欢迎归来, 请输入您的密码以登录:", "登录", "");
			}
			player.SetTeamFixed(NO_TEAM);
			SendClientMessageToAll(COLOR_INFO, std::string(player.GetName() + " 进入服务器").c_str());
			SendDeathMessage(INVALID_PLAYER_ID, playerid, 200);
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
		SendClientMessage(playerid, COLOR_ERROR, "初始化玩家数据时发生错误, 请联系服务器管理员");
		return false;
	}
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDisconnect(int playerid, int reason) {
	try {
		Streamer_OnPlayerDisconnect(playerid, reason);
		Profile& player =  ProfileMgr[playerid];
		if(reason != 0 /* timeout */ && player.IsSignedIn())
			player.Sync();
		int playerteamid = player.GetTeamFixed();
		if(playerteamid != NO_TEAM)
			TeamMgr[TeamMgr.GetNameByID(playerteamid)].Quit(ProfileMgr[playerid]);
		ProfileMgr.Remove(playerid);
		SendClientMessageToAll(COLOR_INFO, std::string(GetPlayerName(playerid) + " 离开服务器").c_str());
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
		SendClientMessage(playerid, COLOR_ERROR, "处理命令时发生未知错误, 请联系服务器管理员");
		return true;
	}
	return false;
}

#define KEY_HOLDING(x) ((newkeys & (x)) == (x))
#define KEY_PRESSED(x) (((newkeys & (x)) == (x)) && ((oldkeys & (x)) != (x)))
#define KEY_RELEASED(x) (((newkeys & (x)) != (x)) && ((oldkeys & (x)) == (x)))

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys) {
	try {
		Profile& player = ProfileMgr[playerid];
		// 车辆加速
		if((KEY_PRESSED(KEY_FIRE) || KEY_PRESSED(KEY_ACTION)) && IsPlayerInAnyVehicle(playerid))
			AddVehicleComponent(GetPlayerVehicleID(playerid), 1010);
		// 角色技能
		if(KEY_PRESSED(KEY_NO)) {
			int target = GetPlayerTargetPlayer(playerid);
			if(player.GetRole().CanPerformSkill()) {
				if(player.GetRole().MustHaveTarget()) {
					if(target != INVALID_PLAYER_ID)
						player.GetRole().PerformSpecialSkill(ProfileMgr[target]);
					else
						throw std::runtime_error("该技能必须有施放对象");
				} else {
					player.GetRole().PerformSpecialSkill(player);
				}
				player.SendChatMessage(COLOR_SUCC, "技能施放成功");
			} else {
				throw std::runtime_error("技能尚在冷却");
			}
		}
	} catch(std::runtime_error& e) {
		SendClientMessage(playerid, COLOR_ERROR, e.what());
		return true;
	} catch(...) {
		SendClientMessage(playerid, COLOR_ERROR, "处理按键时发生未知错误, 请联系服务器管理员");
		return true;
	}
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
		SendClientMessage(playerid, COLOR_ERROR, "处理对话框时发生未知错误, 请联系服务器管理员");
		return true;
	}
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerClickMap(int playerid, float x, float y, float z) {
	SetPlayerPos(playerid, x, y, z);
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDeath(int playerid, int killerid, int reason) {
	GivePlayerMoney(playerid, -500);
	SendDeathMessage(killerid, playerid, reason);
	try {
		Profile& player = ProfileMgr[playerid];
		player.GetRole().OnDeath();
		// 掉落武器
		int dropweapon[2];
		Coordinate3D deadpos = player.GetPos();
		boost::mt19937 engine;
		boost::uniform_real<float> range(-2.5, 2.5);
		boost::variate_generator<boost::mt19937&, boost::uniform_real<float>> genoffset(engine, range);
		for(int i = 0; i < 13; i++) {
			GetPlayerWeaponData(playerid, i, &dropweapon[0], &dropweapon[1]);
			if(dropweapon[0] != 0)
				PickupManager::GetInstance().Add(PickupManager::MemberPtr(new WeaponPickup(dropweapon[0], dropweapon[1],
				deadpos.x + genoffset(), deadpos.y + genoffset(), deadpos.z)));
		}
		// 连杀奖励
		if(killerid != INVALID_PLAYER_ID) {
			Profile& killer = ProfileMgr[killerid];
			killer.GiveScore(1);
			killer.GiveMoney(1000);
			Coordinate3D pos = GenerateDirectionalPoint(killer, 3.0f);
			PickupManager::MemberPtr ptr;
			int kills = killer.KillCounter();
			switch(kills) {
			case 1:
				return true;

			case 2:
				ptr.reset(new MedicalPickup(30.0f, pos.x, pos.y, pos.z));
				break;

			case 3:
				ptr.reset(new WeaponPickup(16, 3, pos.x, pos.y, pos.z));
				break;

			case 4:
				ptr.reset(new WealthPickup(2500, 5, pos.x, pos.y, pos.z));
				break;

			default:
				ptr.reset(new WeaponPickup(38, 15, pos.x, pos.y, pos.z));
				break;
			}
			PickupManager::GetInstance().Add(ptr);
			SendClientMessageToAll(COLOR_POWDERBLUE, std::string("玩家 " + killer.GetName() + " 连续杀敌 "
				+ boost::lexical_cast<std::string>(kills) + " 人").c_str());
		}
	} catch(std::runtime_error& e) {
		SendClientMessage(playerid, COLOR_ERROR, e.what());
		return true;
	} catch(...) {
		SendClientMessage(playerid, COLOR_ERROR, "处理杀敌信息时发生未知错误, 请联系服务器管理员");
		return true;
	}
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerRequestSpawn(int playerid) {
	ShowPlayerDialog(playerid, DIALOG_TEAM_SELECT, DIALOG_STYLE_LIST, "请选择您的阵营",  "Cops\nCriminals", "确定", "");
	return false;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerSpawn(int playerid) {
	try {
		ProfileMgr[playerid].GetRole().OnSpawn();
		Waypoint spawnpoint("_map_spawnpoint_" + TeamMgr.GetNameByID(ProfileMgr[playerid].GetTeamFixed()));
			spawnpoint.PerformTeleport(playerid);
	} catch(std::runtime_error& e) {
		SendClientMessage(playerid, COLOR_ERROR, e.what());
	} catch(...) {
		SendClientMessage(playerid, COLOR_ERROR, "无法出生在团队出生点");
	}
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerText(int playerid, const char *text) {
	SetPlayerChatBubble(playerid, text, GetPlayerColor(playerid), 100.0f, 5000);
	return true;
}

STREAMER_CALLBACK void OnPlayerEnterDynamicArea(int playerid, int areaid) {
	return;
}

STREAMER_CALLBACK void OnPlayerLeaveDynamicArea(int playerid, int areaid) {
	return;
}

STREAMER_CALLBACK void OnDynamicObjectMoved(int objectid) {
	return;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerPickUpPickup(int playerid, int pickupid) {
	try {
		int streamerid = Streamer_OnPlayerPickUpPickup(playerid, pickupid);
		if(streamerid != 0)
			PickupManager::GetInstance().Exec(playerid, streamerid);
	} catch(std::runtime_error &e) {
		SendClientMessage(playerid, COLOR_ERROR, e.what());
	} catch(...) {
		SendClientMessage(playerid, COLOR_ERROR, "处理 Pickup 时发生错误");
	}
	return true;
}
