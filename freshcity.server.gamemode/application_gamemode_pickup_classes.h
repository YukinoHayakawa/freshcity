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

#ifndef FRESHCITY_APPLICATION_GAMEMODE_PICKUP_CLASSES
#define FRESHCITY_APPLICATION_GAMEMODE_PICKUP_CLASSES

#include "application_gamemode_pickup.h"

// MedicalPickup
class MedicalPickup : public Pickup {
private:
	float _health;

public:
	MedicalPickup(float health, float x, float y, float z);
	void Effect(Profile& player);
};

// WealthPickup
class WealthPickup : public Pickup {
private:
	int _money;
	int _score;

public:
	WealthPickup(int money, int score, float x, float y, float z);
	void Effect(Profile& player);
};

// WeaponPickup
class WeaponPickup : public Pickup {
private:
	int _weaponid, _ammo;

public:
	WeaponPickup(int weaponid, int ammo, float x, float y, float z);
	void Effect(Profile& player);
};

// TurfWarTrigger
class TurfWarTrigger : public Pickup {
private:
	int _zoneid;

public:
	TurfWarTrigger(int zoneid, float x, float y, float z);
	void Effect(Profile& player);
};

// TeleportTrigger
class TeleportTrigger : public Pickup {
private:
	mongo::OID _waypoint;

public:
	TeleportTrigger(const mongo::OID& waypoint, float x, float y, float z);
	void Effect(Profile& player);
};

#endif
