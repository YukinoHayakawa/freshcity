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
#include "application_gamemode_item_pickup_classes.h"
#include "application_algorithms.h"
#include "application_gamemode_manager_classes.h"
#include "application_data_waypoint.h"

// MedicalPickup
MedicalPickup::MedicalPickup(float health, float x, float y, float z)
	: DynamicPickup(1240, 1, x, y, z, true), _health(health) {}

void MedicalPickup::Effect(Profile& player) {
	player.GiveHealth(_health);
	player.PlaySound(5201);
}

// WealthPickup
WealthPickup::WealthPickup(int money, int score, float x, float y, float z)
	: DynamicPickup(1550, 1, x, y, z, true), _money(money), _score(score) {}

void WealthPickup::Effect(Profile& player) {
	player.GiveMoney(_money);
	player.GiveScore(_score);
	player.PlaySound(5201);
}

// WeaponPickup
WeaponPickup::WeaponPickup(int weaponid, int ammo, float x, float y, float z)
	: _weaponid(weaponid), _ammo(ammo),
	DynamicPickup(ConvertWeaponIDToModelID(weaponid), 1, x, y, z, true) {}

void WeaponPickup::Effect(Profile& player) {
	player.GiveWeapon(_weaponid, _ammo);
	player.PlaySound(5201);
}

// TurfWarTrigger
TurfWarTrigger::TurfWarTrigger(int zoneid, float x, float y, float z)
	: _zoneid(zoneid), DynamicPickup(1314, 1, x, y, z, false) {}

void TurfWarTrigger::Effect(Profile& player) {
	GangZoneItem& gz(GangZoneMgr[_zoneid]);
	if(player.GetTeamId() != gz.GetOwner()) {
		Team& team(TeamMgr[gz.GetOwner()]);
		if(!team.HasOnlineMember())
			player.SendChatMessage(COLOR_ERROR, "对方没有玩家在线, 不能发动帮派战争");
		else
			gz.StartWar(player);
	}
}

// TeleportTrigger
TeleportTrigger::TeleportTrigger(const mongo::OID& waypoint, float x, float y, float z)
	: _waypoint(waypoint), DynamicPickup(1318, 1, x, y, z, false) {}

void TeleportTrigger::Effect(Profile& player) {
	Waypoint target(_waypoint);
	target.PerformTeleport(player.GetId());
}

// PropertyMarker
PropertyMarker_OnSale::PropertyMarker_OnSale(const mongo::OID& property, float x, float y, float z)
	: _property(property), DynamicPickup(1273, 1, x, y, z, false) {}

void PropertyMarker_OnSale::Effect(Profile& player) {
	if(player.HasVar("property_lastviewtime") && time(0) - player.GetVar<__int64>("property_lastviewtime") < 5) return;
	player.SetVar("property_lastviewtime", time(0));
	player.SetVar("property_lastviewed", _property);
	Property& prop(PropertyMgr[_property]);
	std::stringstream str;
	str << prop.GetName() << "\n售价: $" << prop.GetValue() << "\n每天收益: $" << prop.GetProfit();
	DlgMgr.Show(DIALOG_PROPERTY_ONSALE, str.str(), player.GetId());
}

PropertyMarker_Sold::PropertyMarker_Sold(const mongo::OID& property, float x, float y, float z)
	: _property(property), DynamicPickup(1272, 1, x, y, z, false) {}

void PropertyMarker_Sold::Effect(Profile& player) {
	if(player.HasVar("property_lastviewtime") && time(0) - player.GetVar<__int64>("property_lastviewtime") < 5) return;
	player.SetVar("property_lastviewtime", time(0));
	player.SetVar("property_lastviewed", _property);
	Property& prop(PropertyMgr[_property]);
	std::stringstream str;
	if(prop.GetOwner() == player.GetUniqueID()) {
		str << "领取收益\n售出\n------------------------\n" << prop.GetName() << "\n售价: $"
			<< prop.GetValue() << "\n每天收益: $" << prop.GetProfit();
		DlgMgr.Show(DIALOG_PROPERTY_SOLD, str.str(), player.GetId());
	} else {
		str << prop.GetName() << "\n售价: $" << prop.GetValue() << "\n每天收益: $" << prop.GetProfit();
		DlgMgr.Show(DIALOG_PROPERTY_INFO, str.str(), player.GetId());
	}
}
