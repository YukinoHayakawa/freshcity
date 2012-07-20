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
#include "application_gamemode_role_medic.h"
#include "application_gamemode_colordefinitions.h"
#include <sampgdk/a_players.h>

Medic::Medic() : Role(180, true) {}

void Medic::OnSpawn(int playerid) {
	SendClientMessage(playerid, COLOR_CYAN, "你的职业为 医师");
	SetPlayerHealth(playerid, 100.0f);
	GivePlayerWeapon(playerid, 24, 100);
	GivePlayerWeapon(playerid, 25, 100);
	GivePlayerWeapon(playerid, 29, 100);
}

void Medic::PerformSpecialSkill(int targetid) {
	float health;
	GetPlayerHealth(targetid, &health);
	SetPlayerHealth(targetid, health + 50.0f);
	Role::PerformSpecialSkill(targetid);
}
