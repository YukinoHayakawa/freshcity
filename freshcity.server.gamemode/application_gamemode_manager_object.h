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

#ifndef FRESHCITY_APPLICATION_GAMEMODE_MANAGER_OBJECT
#define FRESHCITY_APPLICATION_GAMEMODE_MANAGER_OBJECT

#include "application_gamemode_manager_base.h"
#include "application_data_object.h"

class ObjectManager : public ItemManager<ObjectManager, int, DynamicObject> {
public:
	bool Add(MemberPtr& item) {
		return ItemManager::Add(item->GetID(), item);
	}
};

#endif
