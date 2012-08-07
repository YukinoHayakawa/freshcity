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
#include <sampgdk/a_samp.h>
#include <sampgdk/a_vehicles.h>
#include <sampgdk/plugin.h>
#include "basic_debug_logging.h"
#include "application_gamemode_manager_classes.h"
#include <boost/algorithm/string.hpp>
#include "basic_algorithm_math.h"
#include "application_gamemode_item_pickup_classes.h"
#include "application_algorithms.h"
#include <boost/random.hpp>
#include "application_gamemode_role_classes.h"
#include "application_gamemode_sysmsgqueue.h"
#include "basic_debug_timer.h"
#include "application_data_waypoint.h"
#include "application_gamemode_callback_timer.h"

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
	//sampgdk_process_timers();
	Streamer_ProcessTick();
}

PLUGIN_EXPORT bool PLUGIN_CALL OnGameModeInit() {
	RUN_TIME_COUNTER;
	SetGameModeText("ShadowBlue");
	for(int i = 1; i < 299; ++i) 
		AddPlayerClass(i, 1497.07f, -689.485f, 94.956f, 180.86f, 0, 0, 0, 0, 0, 0);
	try {
		TeamMgr.LoadAllFromDatabase();
		GangZoneMgr.LoadAllFromDatabase();
		LoadAllTeleportTriggerFromDatabase();
		//ProfileMgr.Add(-1);
		CreateTimer(TimerCallback_ChangeWeather, 0, 300000, true);
		CreateTimer(TimerCallback_ChangeTime, 0, 60000, true);
	} catch(std::runtime_error& e) {
		LOG_ERROR(e.what());
	} catch(mongo::UserException& e) {
		LOG_ERROR(e.what());
	} catch(...) {
		LOG_ERROR("发生未知错误");
	}
	LOG_INFO("Freshcity Gamemode has been inited");
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerConnect(int playerid) {
	RUN_TIME_COUNTER;
	try {
		Streamer_OnPlayerConnect(playerid);
		SendClientMessage(playerid, COLOR_INFO, "TennenColl's Teststation");
		try {
			ProfileMgr.Add(playerid);
			Profile& player = ProfileMgr[playerid];
			player.SetSignedIn(false);
			if(player.IsEmpty()) {
				player.SetColor(RandomRGBAColor());
				DlgMgr.Show(DIALOG_PROFILE_REGISTER, "请输入您的密码", playerid);
			} else {
				if(player.IsBannedForGame()) {
					player.SendChatMessage(COLOR_ERROR, "你已经被服务器封禁");
					player.KickNow();
				} else
					DlgMgr.Show(DIALOG_PROFILE_LOGIN, "欢迎归来, 请输入您的密码以登录", playerid);
			}
			player.SetTeam(NO_TEAM);
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
	RUN_TIME_COUNTER;
	try {
		Streamer_OnPlayerDisconnect(playerid, reason);
		Profile& player =  ProfileMgr[playerid];
		if(reason != 0 /* timeout */ && player.IsSignedIn())
			player.Sync();
		mongo::OID playerteamid = player.GetTeamId();
		if(playerteamid.isSet())
			TeamMgr[playerteamid].Quit(player);
		SendClientMessageToAll(COLOR_INFO, std::string(player.GetName() + " 离开服务器").c_str());
		SendDeathMessage(INVALID_PLAYER_ID, playerid, 201);
		ProfileMgr.Remove(playerid);
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
	RUN_TIME_COUNTER;
	LOG_DEBUG(cmdtext);
	char cmdname[128] = {0}, cmdline[128] = {0};
	sscanf(cmdtext, "%s%*[ ]%[^\0]", cmdname, cmdline);
	try {
		CmdMgr.Exec(playerid, boost::to_lower_copy(std::string(&cmdname[1])), cmdline);
	} catch(std::runtime_error& e) {
		SendClientMessage(playerid, COLOR_ERROR, e.what());
	} catch(...) {
		SendClientMessage(playerid, COLOR_ERROR, "处理命令时发生未知错误, 请联系服务器管理员");
	}
	return true;
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
			RUN_TIME_COUNTER;
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
	} catch(...) {
		SendClientMessage(playerid, COLOR_ERROR, "处理按键时发生未知错误, 请联系服务器管理员");
	}
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnDialogResponse(int playerid, int dialogid, int response, int listitem, const char *inputtext) {
	RUN_TIME_COUNTER;
	LOG_DEBUG("DialogID: " << dialogid << " Response: " << response << " Listitem: " << listitem << " Input: " << inputtext);
	try {
		DlgMgr.Exec(playerid, response == 1, dialogid, listitem, inputtext);
	} catch(std::runtime_error& e) {
		SendClientMessage(playerid, COLOR_ERROR, e.what());
	} catch(...) {
		SendClientMessage(playerid, COLOR_ERROR, "处理对话框时发生未知错误, 请联系服务器管理员");
	}
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDeath(int playerid, int killerid, int reason) {
	RUN_TIME_COUNTER;
	GivePlayerMoney(playerid, -500);
	SendDeathMessage(killerid, playerid, reason);
	try {
		Profile& player = ProfileMgr[playerid];
		player.GetRole().OnDeath();

		// 掉落武器
		int dropweapon[2];
		Coordinate3D deadpos = player.GetPos();
		boost::mt19937 engine;
		float radius = CONFIG_FLOAT("EffectiveItem.weapondropradius");
		boost::uniform_real<float> range(radius * -1, radius);
		boost::variate_generator<boost::mt19937&, boost::uniform_real<float>> genoffset(engine, range);
		for(int i = 0; i < 13; ++i) {
			GetPlayerWeaponData(playerid, i, &dropweapon[0], &dropweapon[1]);
			if(dropweapon[0] != 0)
				PickupMgr.Add(PickupManager::MemberPtr(new WeaponPickup(dropweapon[0], dropweapon[1],
				deadpos.x + genoffset(), deadpos.y + genoffset(), deadpos.z)));
		}
		
		if(killerid != INVALID_PLAYER_ID) {
			Profile& killer = ProfileMgr[killerid];

			// 连杀奖励
			killer.GiveScore(1);
			killer.GiveMoney(1000);
			Coordinate3D pos = GenerateDirectionalPoint(killer, CONFIG_FLOAT("EffectiveItem.distance"));
			PickupManager::MemberPtr ptr;
			int kills = killer.KillCounter();
			std::stringstream killmsg(killer.GetName());
			switch(kills) {
			case 1:
				break;

			case 2:
				killmsg << " double kill";
				ptr.reset(new MedicalPickup(CONFIG_FLOAT("EffectiveItem.medicalpickup"), pos.x, pos.y, pos.z));
				break;

			case 3:
				killmsg << " triple kill";
				ptr.reset(new WeaponPickup(16, 3, pos.x, pos.y, pos.z));
				break;

			case 4:
				killmsg << " quadra kill";
				ptr.reset(new WealthPickup(CONFIG_INT("EffectiveItem.wealthpickupmoney"),
					CONFIG_INT("EffectiveItem.wealthpickupscore"), pos.x, pos.y, pos.z));
				break;

			case 5:
				killmsg << " penta kill";
				ptr.reset(new WeaponPickup(38, 15, pos.x, pos.y, pos.z));
				break;

			default:
				killmsg << " kills " << kills << " in a row";
				ptr.reset(new WeaponPickup(38, 15, pos.x, pos.y, pos.z));
				break;
			}
			if(kills > 1)
				SystemMessageQueue::GetInstance().PushMessage(killmsg.str());

			// TurfWar Counter
			if(IsPlayerInAnyDynamicArea(playerid)) {
				int zoneid = GangZoneMgr.GetPointInWhichZone(player.GetPos());
				if(zoneid != -1) {
					GangZoneItem& gz = GangZoneMgr[zoneid];
					if(player.GetTeamId() == gz.GetOwner() && killer.GetTeamId() == gz.GetAttacker())
						gz.CountMemberDeath();
					if(player.GetTeamId() == gz.GetAttacker() && killer.GetTeamId() == gz.GetOwner())
						gz.CountEnemyKill();
				}
			}
		}
	} catch(std::runtime_error& e) {
		SendClientMessage(playerid, COLOR_ERROR, e.what());
	} catch(...) {
		SendClientMessage(playerid, COLOR_ERROR, "处理杀敌信息时发生未知错误, 请联系服务器管理员");
	}
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerRequestSpawn(int playerid) {
	std::string teams;
	MANAGER_FOREACH(TeamManager) teams.append(iter->first).append("\t").append(iter->second->GetName()).append("\n");
	DlgMgr.Show(DIALOG_TEAM_SELECT, teams, playerid);
	return false;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerSpawn(int playerid) {
	try {
		Profile& player = ProfileMgr[playerid];
		player.GetRole().OnSpawn();
		std::stringstream gangzones;
		MANAGER_FOREACH(GangZoneManager) {
			iter->second->Redraw();
			gangzones << iter->first << "\t" << iter->second->GetName() << "\n";
		}
		DlgMgr.Show(DIALOG_GANGZONE_CHOOSETOSPAWN, gangzones.str(), playerid);
	} catch(std::runtime_error& e) {
		SendClientMessage(playerid, COLOR_ERROR, e.what());
	} catch(...) {
		SendClientMessage(playerid, COLOR_ERROR, "无法出生在团队出生点");
	}
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerText(int playerid, const char *text) {
	SetPlayerChatBubble(playerid, text, GetPlayerColor(playerid), 100.0f, 5000);
	Profile& p(ProfileMgr[playerid]);
	Team& t(TeamMgr[p.GetTeamId()]);
	std::stringstream chat;
	chat << ColorToEmbeddingString(t.GetColor()) << "[" << t.GetName() << "]" <<
		ColorToEmbeddingString(p.GetColor()) << p.GetNickname() << "{ffffff}: " << text;
	SendClientMessageToAll(COLOR_WHITE, chat.str().c_str());
	return false;
}

STREAMER_CALLBACK void OnPlayerEnterDynamicArea(int playerid, int areaid) {
	try {
		DynAreaMgr[areaid].OnPlayerEnter(ProfileMgr[playerid]);
	} catch(std::runtime_error &e) {
		SendClientMessage(playerid, COLOR_ERROR, e.what());
	} catch(...) {
		SendClientMessage(playerid, COLOR_ERROR, "OnPlayerEnterDynamicArea 发生错误");
	}
	return;
}

STREAMER_CALLBACK void OnPlayerLeaveDynamicArea(int playerid, int areaid) {
	try {
		DynAreaMgr[areaid].OnPlayerExit(ProfileMgr[playerid]);
	} catch(std::runtime_error &e) {
		SendClientMessage(playerid, COLOR_ERROR, e.what());
	} catch(...) {
		SendClientMessage(playerid, COLOR_ERROR, "OnPlayerLeaveDynamicArea 发生错误");
	}
	return;
}

STREAMER_CALLBACK void OnDynamicObjectMoved(int objectid) {
	return;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerPickUpPickup(int playerid, int pickupid) {
	RUN_TIME_COUNTER;
	try {
		int streamerid = Streamer_OnPlayerPickUpPickup(playerid, pickupid);
		if(streamerid != 0)
			PickupMgr.Exec(playerid, streamerid);
	} catch(std::runtime_error &e) {
		SendClientMessage(playerid, COLOR_ERROR, e.what());
	} catch(...) {
		SendClientMessage(playerid, COLOR_ERROR, "处理 Pickup 时发生错误");
	}
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerEditObject(int playerid, bool playerobject, int objectid, int response, float fX, float fY, float fZ, float fRotX, float fRotY, float fRotZ) {
	Streamer_OnPlayerEditObject(playerid, playerobject, objectid, response, fX, fY, fZ, fRotX, fRotY, fRotZ);
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerClickMap(int playerid, float fX, float fY, float fZ) {
	RUN_TIME_COUNTER;
	try {
		Profile& player(ProfileMgr[playerid]);

		// GangZone Creation
		if(player.HasVar("gz_create")) {
			GangZoneCreationInfo& data(player.GetVar<GangZoneCreationInfo>("gz_create"));
			if((data.step & GANGZONE_CREATE_MIN) != GANGZONE_CREATE_MIN) {
				data.min = CoordinatePlane(fX, fY);
				data.step |= GANGZONE_CREATE_MIN;
			} else if((data.step & GANGZONE_CREATE_MAX) != GANGZONE_CREATE_MAX) {
				data.max = CoordinatePlane(fX, fY);
				data.step |= GANGZONE_CREATE_MAX;
				player.SendChatMessage(COLOR_SUCC, "已确定 GangZone 范围");
			}
		}
	} catch(std::runtime_error &e) {
		SendClientMessage(playerid, COLOR_ERROR, e.what());
	} catch(...) {
		SendClientMessage(playerid, COLOR_ERROR, "OnPlayerClickMap 发生错误");
	}
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerClickPlayer(int playerid, int clickedplayerid, int source) {
	RUN_TIME_COUNTER;
	try {
		Profile& player(ProfileMgr[playerid]);
		player.SetVar("player_lastclicked", clickedplayerid);
		if(playerid != clickedplayerid)
			DlgMgr.Show(DIALOG_PLAYER_MAIN, "发送消息", playerid);
		else
			DlgMgr.Show(DIALOG_PROFILE_SETTING, "设置昵称\n同步数据\n更换队伍\n自杀", playerid); 
	} catch(std::runtime_error &e) {
		SendClientMessage(playerid, COLOR_ERROR, e.what());
	} catch(...) {
		SendClientMessage(playerid, COLOR_ERROR, "OnPlayerClickPlayer 发生错误");
	}
	return true;
}
