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
#include <sampgdk/plugin.h>
#include "basic_debug_logging.h"
#include "basic_algorithm_wchar.h"
#include "application_gamemode_manager_profile.h"
#include "application_gamemode_manager_command.h"
#include "application_gamemode_colordefinitions.h"
#include <boost/algorithm/string.hpp>

ProfileManager& ProfileMgr(ProfileManager::GetInstance());

PLUGIN_EXPORT bool PLUGIN_CALL OnGameModeInit() {
	SetGameModeText("Freshcity");
	AddPlayerClass(0, 1958.3783f, 1343.1572f, 15.3746f, 269.1425f, 0, 0, 0, 0, 0, 0);
	LOG_INFO("Freshcity Gamemode 已载入");
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerConnect(int playerid) {
	try {
		SendClientMessage(playerid, COLOR_INFO, "欢迎来到 TennenColl 的开发服务器");
		try {
			ProfileMgr.Add(playerid);
			ProfileMgr.SetAuthed(playerid, false);
			Profile& player = ProfileMgr[playerid];
			if(player.IsEmpty()) {
				player.SendChatMessage(COLOR_INFO, "你还没有注册, 请 /register <密码> 来创建新用户.");
			} else {
				player.SendChatMessage(COLOR_WARN, "欢迎回来, " + player.GetName() + " . 请执行 /login <密码> 以登录.");
			}
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
		ProfileMgr.Remove(playerid);
	} catch(...) {
		return false;
	}
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerRequestClass(int playerid, int classid) {
	SetPlayerPos(playerid, 1958.3783f, 1343.1572f, 15.3746f);
	SetPlayerCameraPos(playerid, 1958.3783f, 1343.1572f, 15.3746f);
	SetPlayerCameraLookAt(playerid, 1958.3783f, 1343.1572f, 15.3746f, CAMERA_CUT);
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerCommandText(int playerid, const char *cmdtext) {
	char cmdname[128] = {0}, cmdline[128] = {0};
	sscanf(cmdtext, "%s%s", cmdname, cmdline);
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
