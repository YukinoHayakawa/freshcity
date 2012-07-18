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
#include "application_gamemode_manager_dialog.h"
#include "application_gamemode_manager_team.h"
#include "application_gamemode_dialogdefinitions.h"
#include "application_data_waypoint.h"

#define DIALOG(x) void Dlg##x(Profile& player, bool response, int listitem, const char* inputtext)

DIALOG(SelectTeam) {
	if(player.GetTeamFixed() != TeamManager::GetInstance()[inputtext].GetIngameID()) {
		if(player.GetTeamFixed() != NO_TEAM)
			TeamManager::GetInstance()[TeamManager::GetInstance().GetNameByID(player.GetTeamFixed())].Quit(player);
		TeamManager::GetInstance()[inputtext].Join(player);
	}
	Waypoint spawnpoint("_map_spawnpoint_" + std::string(inputtext));
	spawnpoint.PerformTeleport(player.GetId());
}

#define REGDLG(x, y) DlgMgr.Add(x, y)

bool RegisterPlayerDlgs() {
	DialogManager& DlgMgr = DialogManager::GetInstance();
	REGDLG(DIALOG_TEAM_SELECT,		DlgSelectTeam);
	return true;
}

#undef REGDLG

void* PlayerDlgInit((void*)RegisterPlayerDlgs());
