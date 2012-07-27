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
#include "application_gamemode_role_classes.h"
#include "application_gamemode_colordefinitions.h"
#include <sampgdk/a_players.h>
#include "application_data_profile.h"
#include "application_algorithms.h"
#include <sampgdk/a_vehicles.h>
#include "application_gamemode_object.h"

// Assault
Assault::Assault(Profile& player) : Role(player, 60, true) {}

void Assault::OnSpawn() {
	_player.SendChatMessage(COLOR_CYAN,
		"你的职业为[突击队员], 瞄准目标按下~k~~CONVERSATION_NO~键可将对方抓到面前, 施放间隔1分钟");
	_player.SetHealth(100.0f);
	_player.SetArmour(100.0f);
	_player.GiveWeapon(24, 100);
	_player.GiveWeapon(25, 100);
	_player.GiveWeapon(29, 100);
}

void Assault::PerformSpecialSkill(Profile& target) {
	Coordinate3D pos = GenerateDirectionalPoint(_player, 2.5);
	target.SetPos(pos.x, pos.y, pos.z);
	target.SendChatMessage(COLOR_WARN, "你已被 Assault 捕获, 不要尝试无谓的挣脱!");
	Role::PerformSpecialSkill(target);
}

// Medic
Medic::Medic(Profile& player) : Role(player, 60, true) {}

void Medic::OnSpawn() {
	_player.SendChatMessage(COLOR_CYAN,
		"你的职业为[医师], 瞄准目标按下~k~~CONVERSATION_NO~键可治愈对方50点血, 施放间隔1分钟");
	_player.SetHealth(100.0f);
	_player.SetArmour(0.0f);
	_player.GiveWeapon(23, 100);
	_player.GiveWeapon(32, 100);
}

void Medic::PerformSpecialSkill(Profile& target) {
	target.GiveHealth(50.0f);
	Role::PerformSpecialSkill(target);
}

// Mechanic
Mechanic::Mechanic(Profile& player) : Role(player, 180, false) {}

void Mechanic::OnSpawn() {
	_player.SendChatMessage(COLOR_CYAN,
		"你的职业为[机修工], 在车内按下~k~~CONVERSATION_NO~键可修复自己所乘车辆, 施放间隔3分钟");
	_player.SetHealth(100.0f);
	_player.SetArmour(0.0f);
	_player.GiveWeapon(9, 1);
	_player.GiveWeapon(25, 100);
	_player.GiveWeapon(28, 100);
}

void Mechanic::PerformSpecialSkill(Profile& target) {
	if(!IsPlayerInAnyVehicle(_player.GetId()))
		throw std::runtime_error("必须在车内才能进行维修");
	RepairVehicle(_player.GetVehicleID());
	Role::PerformSpecialSkill(target);
}

// Engineer
Engineer::Engineer(Profile& player) : Role(player, 60, false) {}

void Engineer::OnSpawn() {
	_player.SendChatMessage(COLOR_CYAN,
		"你的职业为[工程师], 徒步时按下~k~~CONVERSATION_NO~键可在面前修建哨塔一座并会清除上次的建筑, 施放间隔1分钟");
	_player.SetHealth(100.0f);
	_player.SetArmour(0.0f);
	_player.GiveWeapon(16, 3);
	_player.GiveWeapon(25, 100);
	_player.GiveWeapon(22, 100);
}

void Engineer::OnDeath() {
	_obj.reset();
}

void Engineer::PerformSpecialSkill(Profile& target) {
	if(IsPlayerInAnyVehicle(_player.GetId()))
		throw std::runtime_error("不能在车内进行建设");
	Coordinate3D pos = GenerateDirectionalPoint(_player, 10.0f);
	_obj.reset(new DynamicObject(3279, pos.x, pos.y, pos.z - 1, 0.0f, 0.0f, 0.0f));
	Role::PerformSpecialSkill(target);
}
