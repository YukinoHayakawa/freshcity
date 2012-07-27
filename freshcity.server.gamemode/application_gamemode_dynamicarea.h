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

#ifndef FRESHCITY_APPLICATION_GAMEMODE_DYNAMICAREA
#define FRESHCITY_APPLICATION_GAMEMODE_DYNAMICAREA

#include "application_dependency_streamer.h"
#include "application_data_profile.h"

class DynamicArea {
protected:
	int _id;

public:
	DynamicArea(int id) : _id(id) {}
	virtual void OnPlayerEnter(Profile& player) {}
	virtual void OnPlayerExit(Profile& player) {}

	int GetID() const {
		return _id;
	}

	~DynamicArea() {
		DestroyDynamicArea(_id);
	}

	void TogglePlayer(int playerid, bool toggle) {
		TogglePlayerDynamicArea(playerid, _id, toggle);
	}

	bool IsPlayerIn(int playerid) const {
		return IsPlayerInDynamicArea(playerid, _id);
	}

	bool IsPointIn(float x, float y, float z) const {
		return IsPointInDynamicArea(_id, x, y, z);
	}

	void AttachToObject(int objectid, int type = STREAMER_OBJECT_TYPE_DYNAMIC, int playerid = INVALID_PLAYER_ID) {
		AttachDynamicAreaToObject(_id, objectid, type, playerid);
	}

	void AttachToPlayer(int playerid) {
		AttachDynamicAreaToPlayer(_id, playerid);
	}

	void AttachToVehicle(int vehicleid) {
		AttachDynamicAreaToVehicle(_id, vehicleid);
	}
};

class DynamicCircle : public DynamicArea {
public:
	DynamicCircle(float x, float y, float size, int worldid = -1, int interiorid = -1, int playerid = -1)
		: DynamicArea(CreateDynamicCircle(x, y, size, worldid, interiorid, playerid)) {}
};

class DynamicRectangle : public DynamicArea {
public:
	DynamicRectangle(float minx, float miny, float maxx, float maxy, int worldid = -1, int interiorid = -1, int playerid = -1)
		: DynamicArea(CreateDynamicRectangle(minx, miny, maxx, maxy, worldid, interiorid, playerid)) {}
};

class DynamicSphere : public DynamicArea {
public:
	DynamicSphere(float x, float y, float z, float size, int worldid = -1, int interiorid = -1, int playerid = -1)
		: DynamicArea(CreateDynamicSphere(x, y, z, size, worldid, interiorid, playerid)) {}
};

class DynamicCube : public DynamicArea {
public:
	DynamicCube(float minx, float miny, float minz, float maxx, float maxy, float maxz, int worldid = -1, int interiorid = -1, int playerid = -1)
		: DynamicArea(CreateDynamicCube(minx, miny, minz, maxx, maxy, maxz, worldid, interiorid, playerid)) {}
};

#endif
