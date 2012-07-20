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
#include "application_gamemode_manager_effectiveitem.h"
#include "application_gamemode_manager_profile.h"

bool EffectiveItemManager::Add(MemberPtr& item) {
	return ItemManager::Add(item->GetID(), item);
}

void EffectiveItemManager::Exec(int playerid, int itemid) {
	MemberMap::const_iterator iter(_members.find(itemid));
	if(iter == _members.end()) throw std::runtime_error("无效对象ID");
	Profile& player = ProfileManager::GetInstance()[playerid];
	iter->second->Effect(player);
	if(iter->second->IsDisposable()) _members.erase(iter);
}
