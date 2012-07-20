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

MedicalPickup::MedicalPickup(float x, float y, float z)
	: Pickup(1240, 1, x, y, z, true) {}

void MedicalPickup::Effect(Profile& player) {
	Coordinate3D pos = player.GetPos();
	float difference = 100 - player.GetHealth();
	if(difference > 10)
		player.SetHealth(player.GetHealth() + 10.0f);
	else if(difference <= 10)
		player.SetHealth(100.0f);
	player.PlaySound(5201, pos.x, pos.y, pos.z);
}
