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
#include "application_gamemode_colordefinitions.h"
#include "application_gamemode_role_classes.h"

#define DIALOG(x) void Dlg##x(Profile& player, bool response, int listitem, const char* inputtext)

DIALOG(SelectTeam) {
	if(player.GetTeamFixed() != TeamManager::GetInstance()[inputtext].GetIngameID()) {
		if(player.GetTeamFixed() != NO_TEAM)
			TeamManager::GetInstance()[TeamManager::GetInstance().GetNameByID(player.GetTeamFixed())].Quit(player);
		TeamManager::GetInstance()[inputtext].Join(player);
	}
	ShowPlayerDialog(player.GetId(), DIALOG_ROLE_SELECT, DIALOG_STYLE_LIST, "选择职业",
		"Assault\nMedic\nMechanic\nEngineer", "确定", "");
}

DIALOG(SelectRole) {
	switch(listitem) {
	case 0:
		player.SetRole(Profile::RolePtr(new Assault(player)));
		break;

	case 1:
		player.SetRole(Profile::RolePtr(new Medic(player)));
		break;

	case 2:
		player.SetRole(Profile::RolePtr(new Mechanic(player)));
		break;

	default:
		player.SendChatMessage(COLOR_ERROR, "其他职业还在开发中");
		ShowPlayerDialog(player.GetId(), DIALOG_ROLE_SELECT, DIALOG_STYLE_LIST, "选择职业",
			"Assault\nMedic\nMechanic\nEngineer", "确定", "");
		break;
	}
	player.Spawn();
}

DIALOG(Register) {
	if(inputtext[0] == 0) throw std::runtime_error("密码不能为空");
	try {
		player.Create(player.GetName(), inputtext);
		player.SendChatMessage(COLOR_SUCC, "注册成功");
		player.SetSignedIn(true);
	} catch(std::runtime_error) {
		throw;
	} catch(...) {
		throw std::runtime_error("注册失败");
	}
}

DIALOG(Login) {
	if(!player.AuthPassword(inputtext)) throw std::runtime_error("密码错误");
	player.SetSignedIn(true);
	player.ApplyDataToPlayer();
	player.SendChatMessage(COLOR_SUCC, "登录成功");
}

#define REGDLG(x, y) DlgMgr.Add(x, y)

bool RegisterPlayerDlgs() {
	DialogManager& DlgMgr = DialogManager::GetInstance();
	REGDLG(DIALOG_TEAM_SELECT,			DlgSelectTeam);
	REGDLG(DIALOG_ROLE_SELECT,			DlgSelectRole);
	REGDLG(DIALOG_PROFILE_REGISTER,		DlgRegister);
	REGDLG(DIALOG_PROFILE_LOGIN,		DlgLogin);
	return true;
}

#undef REGDLG

void* PlayerDlgInit((void*)RegisterPlayerDlgs());
