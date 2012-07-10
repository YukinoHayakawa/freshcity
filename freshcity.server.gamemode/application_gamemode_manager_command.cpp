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

CommandManager::CommandManager() {}

CommandManager::~CommandManager() {}

bool CommandManager::Add(const std::string& cmd, COMMAND_CALLBACK function) {
	if(IsExist(cmd)) return false;
	_cmds.insert(std::make_pair(cmd, CallbackPtr(function)));
	return true;
}

bool CommandManager::IsExist(const std::string& cmd) const {
	return _cmds.find(cmd) != _cmds.end();
}

bool CommandManager::Remove(const std::string& cmd) {
	CommandMap::const_iterator iter(_cmds.find(cmd));
	if(iter == _cmds.end()) return false;
	_cmds.erase(iter);
	return true;
}

void CommandManager::Exec(int playerid, const std::string& cmd, const char* cmdline) const {
	CommandMap::const_iterator iter(_cmds.find(cmd));
	if(iter == _cmds.end()) throw std::runtime_error("²»´æÔÚµÄÃüÁî.");
	iter->second(ProfileManager::GetInstance()[playerid], cmdline);
}

CommandManager& CommandManager::GetInstance() {
	static CommandManager inst;
	return inst;
}
