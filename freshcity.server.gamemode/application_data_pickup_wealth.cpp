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
#include "application_data_pickup_wealth.h"
#include "application_config.h"

WealthPickup::WealthPickup(float x, float y, float z)
	: Pickup(1212, 1, x, y, z, true) {}

void WealthPickup::Effect(Profile& player) {
	player.GiveMoney(CONFIG_INT("EffectiveItem.wealthpickupmoney"));
	player.GiveScore(CONFIG_INT("EffectiveItem.wealthpickupscore"));
	player.PlaySound(5201);
}
