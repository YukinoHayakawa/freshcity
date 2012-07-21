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
#include "application_data_pickup_classes.h"
#include "application_algorithm_weaponidmap.h"

// MedicalPickup
MedicalPickup::MedicalPickup(float health, float x, float y, float z)
	: Pickup(1240, 1, x, y, z, true), _health(health) {}

void MedicalPickup::Effect(Profile& player) {
	player.GiveHealth(_health);
	player.PlaySound(5201);
}

// WealthPickup
WealthPickup::WealthPickup(int money, int score, float x, float y, float z)
	: Pickup(1212, 1, x, y, z, true), _money(money), _score(score) {}

void WealthPickup::Effect(Profile& player) {
	player.GiveMoney(_money);
	player.GiveScore(_score);
	player.PlaySound(5201);
}

// WeaponPickup
WeaponPickup::WeaponPickup(int weaponid, int ammo, float x, float y, float z)
	: _weaponid(weaponid), _ammo(ammo),
	Pickup(ConvertWeaponIDToModelID(weaponid), 1, x, y, z, true) {}

void WeaponPickup::Effect(Profile& player) {
	player.GiveWeapon(_weaponid, _ammo);
	player.PlaySound(5201);
}
