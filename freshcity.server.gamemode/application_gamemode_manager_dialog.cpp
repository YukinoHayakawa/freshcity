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

bool DialogManager::Add(int dialogid, DIALOG_CALLBACK function) {
	return ItemManager::Add(dialogid, MemberPtr(new DialogPtr(function)));
}

void DialogManager::Exec(int playerid, bool response, int dialogid, int listitem, const char* inputtext) {
	MemberMap::const_iterator iter(_members.find(dialogid));
	if(iter == _members.end()) throw std::runtime_error("未注册回调的对话框");
	Profile& player = ProfileManager::GetInstance()[playerid];
	iter->second->operator()(player, response, listitem, inputtext);
}
