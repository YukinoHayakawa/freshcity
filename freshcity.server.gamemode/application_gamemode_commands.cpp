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
#include <sampgdk/a_players.h>

#define CMD(x) void Cmd##x(Profile& player, const char* cmdline)

CMD(Register) {
	if(!player.IsEmpty()) throw std::runtime_error("玩家已注册.");
	if(cmdline == '\0') throw std::runtime_error("密码不能为空.");
	try {
		player.Create(player.GetName(), cmdline);
		player.SendChatMessage(COLOR_SUCC, std::string("注册成功. 登录名: " + player.GetName() + ", 密码(引号内): \"" + cmdline + "\".").c_str());
		ProfileManager::GetInstance().SetAuthed(player.GetId(), true);
	} catch(...) {
		throw std::runtime_error("注册失败.");
	}
}

CMD(Login) {
	if(player.IsEmpty()) throw std::runtime_error("玩家尚未注册.");
	if(ProfileManager::GetInstance().IsAuthed(player.GetId())) throw std::runtime_error("玩家已登录过.");
	if(!player.AuthPassword(cmdline)) throw std::runtime_error("密码错误.");
	ProfileManager::GetInstance().SetAuthed(player.GetId(), true);
	player.ApplyDataToPlayer();
	player.SendChatMessage(COLOR_SUCC, "登录成功.");
}

CMD(LogOut) {
	if(!ProfileManager::GetInstance().IsAuthed(player.GetId())) throw std::runtime_error("玩家尚未登录.");
	ProfileManager::GetInstance().SetAuthed(player.GetId(), false);
	player.SendChatMessage(COLOR_SUCC, "您已登出.");
}

CMD(SaveData) {
	if(!ProfileManager::GetInstance().IsAuthed(player.GetId())) throw std::runtime_error("玩家未登录.");
	player.Sync();
	player.SendChatMessage(COLOR_SUCC, "数据已保存.");
}

CMD(SetSkin) {
	int skinid(0);
	if(sscanf(cmdline, "%d", &skinid) == 0) throw std::runtime_error("用法: /setskin <皮肤ID>");
	player.SetSkin(skinid);
	player.SendChatMessage(COLOR_SUCC, "皮肤已更改.");
}

bool RegisterPlayerCmds() {
	CommandManager& CmdMgr = CommandManager::GetInstance();
	CmdMgr.Add("register", CmdRegister);
	CmdMgr.Add("login", CmdLogin);
	CmdMgr.Add("logout", CmdLogOut);
	CmdMgr.Add("sync", CmdSaveData);
	CmdMgr.Add("setskin", CmdSetSkin);
	return true;
}

void* PlayerCmdInit((void*)RegisterPlayerCmds());
