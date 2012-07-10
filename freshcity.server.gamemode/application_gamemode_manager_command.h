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

#ifndef FRESHCITY_APPLICATION_GAMEMODE_MANAGER_COMMAND
#define FRESHCITY_APPLICATION_GAMEMODE_MANAGER_COMMAND

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/function.hpp>
#include "application_gamemode_manager_profile.h"

typedef void (*COMMAND_CALLBACK)(Profile& player, const char* cmdline);

class CommandManager {
protected:
	typedef boost::function<void(Profile&, const char*)> CallbackPtr;
	typedef boost::unordered_map<std::string, CallbackPtr> CommandMap;
	CommandMap _cmds;
	CommandManager();
	~CommandManager();

public:
	bool Add(const std::string& cmd, COMMAND_CALLBACK function);
	bool IsExist(const std::string& cmd) const;
	bool Remove(const std::string& cmd);
	void Exec(int playerid, const std::string& cmd, const char* cmdline) const;
	static CommandManager& GetInstance();
};

#endif
