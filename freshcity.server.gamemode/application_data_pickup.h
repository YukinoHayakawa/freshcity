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

#ifndef FRESHCITY_APPLICATION_DATA_PICKUP
#define FRESHCITY_APPLICATION_DATA_PICKUP

#include "application_dependency_streamer.h"
#include "application_data_profile.h"
#include "application_gamemode_effectiveitem.h"

class Pickup : public EffectiveItem {
public:
	Pickup(int modelid, int type, float x, float y, float z, bool disposable, int worldid = -1, int interiorid = -1, int playerid = -1, float distance = 100.0f);
	~Pickup();
};

#endif
