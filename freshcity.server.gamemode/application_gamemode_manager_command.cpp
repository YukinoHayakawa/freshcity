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

bool CommandManager::Add(const std::string& cmd, COMMAND_CALLBACK function, int reqlevel, unsigned int flags) {
	return ItemManager::Add(cmd, MemberPtr(new CommandCallbackCell(CommandPtr(function), reqlevel, flags)));
}

#define MATCHREQ(req) ((iter->second->flags & req) == req)

void CommandManager::Exec(int playerid, const std::string& cmd, const char* cmdline) {
	MemberMap::const_iterator iter(_members.find(cmd));
	if(iter == _members.end()) throw std::runtime_error("不存在的命令");
	Profile& player = ProfileManager::GetInstance()[playerid];
	if(!MATCHREQ(NO_REQUIREMENT)) {
		if(MATCHREQ(NEED_REGISTERED) && ProfileManager::GetInstance()[playerid].IsExistInDatabase() == false)
			throw std::runtime_error("此命令仅限已注册玩家使用");
		if(MATCHREQ(NEED_SIGNED_IN) && ProfileManager::GetInstance()[playerid].IsSignedIn() == false)
			throw std::runtime_error("此命令仅限已登录玩家使用");
		if(MATCHREQ(DONOT_REGISTERED) && ProfileManager::GetInstance()[playerid].IsExistInDatabase() == true)
			throw std::runtime_error("此命令仅限未注册玩家使用");
		if(MATCHREQ(DONOT_SIGNED_IN) && ProfileManager::GetInstance()[playerid].IsSignedIn() == true)
			throw std::runtime_error("此命令仅限未登录玩家使用");
	}
	if(iter->second->reqlevel > player.GetAdminLevel())
		throw std::runtime_error("您没有足够管理权限来执行此命令");
	iter->second->ptr(player, cmdline);
}

#undef MATCHREQ
