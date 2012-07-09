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
#include "application_data_profile.h"
#include "basic_algorithm_wchar.h"

PLUGIN_EXPORT bool PLUGIN_CALL OnGameModeInit() {
	SetGameModeText("Freshcity");
	AddPlayerClass(0, 1958.3783f, 1343.1572f, 15.3746f, 269.1425f, 0, 0, 0, 0, 0, 0);
	LOG_INFO("Freshcity Gamemode “—‘ÿ»Î");
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerConnect(int playerid) {
	SendClientMessage(playerid, 0xFFFFFFFF, "Welcome to TennenColl's devserver!");
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerRequestClass(int playerid, int classid) {
	SetPlayerPos(playerid, 1958.3783f, 1343.1572f, 15.3746f);
	SetPlayerCameraPos(playerid, 1958.3783f, 1343.1572f, 15.3746f);
	SetPlayerCameraLookAt(playerid, 1958.3783f, 1343.1572f, 15.3746f, CAMERA_CUT);
	return true;
}

PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerCommandText(int playerid, const char *cmdtext) {
	if (std::strcmp(cmdtext, "/register") == 0) {
		try {
			Profile test(playerid, GetPlayerName(playerid));
			if(test.IsEmpty())
				test.Create(test.GetName(), "TennenColl");
			test.Sync();
		} catch(std::runtime_error& e) {
			LOG_ERROR(e.what());
		} catch(mongo::UserException& e) {
			LOG_ERROR(e.what());
		}
		return true;
	}
	if (std::strcmp(cmdtext, "/pos") == 0) {
		float x, y, z;
		GetPlayerPos(playerid, &x, &y, &z);
		char message[128];
		std::sprintf(message, "You are at (%f, %f, %f)", x, y, z);
		SendClientMessage(playerid, 0xFFFFFFFF, message);
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
