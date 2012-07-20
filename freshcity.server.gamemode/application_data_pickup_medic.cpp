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
#include "application_data_pickup_medic.h"
#include "application_config.h"

MedicalPickup::MedicalPickup(float health, float x, float y, float z)
	: Pickup(1240, 1, x, y, z, true), _health(health) {}

void MedicalPickup::Effect(Profile& player) {
	float difference = 100 - player.GetHealth();
	if(difference > _health)
		player.SetHealth(player.GetHealth() + _health);
	else if(difference <= _health)
		player.SetHealth(100.0f);
	player.PlaySound(5201);
}
