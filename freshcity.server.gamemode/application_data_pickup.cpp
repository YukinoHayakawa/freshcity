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
#include "application_data_pickup.h"

Pickup::Pickup(int modelid, int type, float x, float y, float z, bool disposable, int worldid, int interiorid, int playerid, float distance)
	: EffectiveItem(CreateDynamicPickup(modelid, type, x, y, z, worldid, interiorid, playerid, distance), disposable) {
		if(_id == 0) throw std::runtime_error("Pickup ´´½¨Ê§°Ü");
}

Pickup::~Pickup() {
	if(_id != 0)
		DestroyDynamicPickup(_id);
}
