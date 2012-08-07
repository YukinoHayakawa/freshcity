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

#ifndef FRESHCITY_APPLICATION_GAMEMODE_ITEM_STREAMED
#define FRESHCITY_APPLICATION_GAMEMODE_ITEM_STREAMED

#include "application_dependency_streamer.h"
#include "application_data_profile.h"
#include "application_gamemode_item_effective.h"
#include "application_struct_coordinate.h"

// Area
class DynamicArea {
protected:
	const int _id;

public:
	DynamicArea(int id) : _id(id) {}
	virtual void OnPlayerEnter(Profile& player) {}
	virtual void OnPlayerExit(Profile& player) {}
	int GetID() const { return _id; }
	~DynamicArea() { DestroyDynamicArea(_id); }
	void TogglePlayer(int playerid, bool toggle) { TogglePlayerDynamicArea(playerid, _id, toggle); }
	bool IsPlayerIn(int playerid) const { return IsPlayerInDynamicArea(playerid, _id); }
	bool IsPointIn(float x, float y, float z) const { return IsPointInDynamicArea(_id, x, y, z); }
	void AttachToObject(int objectid, int type = STREAMER_OBJECT_TYPE_DYNAMIC, int playerid = INVALID_PLAYER_ID) { AttachDynamicAreaToObject(_id, objectid, type, playerid); }
	void AttachToPlayer(int playerid) { AttachDynamicAreaToPlayer(_id, playerid); }
	void AttachToVehicle(int vehicleid) { AttachDynamicAreaToVehicle(_id, vehicleid); }
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

// Object
class DynamicObject : public EffectiveItem {
public:
	DynamicObject(int modelid, float x, float y, float z, float rx, float ry, float rz,
		int worldid = -1, int interiorid = -1, int playerid = -1, float streamdistance = 100.0)
		: EffectiveItem(CreateDynamicObject(modelid, x, y, z, rx, ry, rz, worldid, interiorid, playerid, streamdistance), false) {}
	~DynamicObject() { DestroyDynamicObject(_id); }
	bool SetPos(Coordinate3D& pos) { return SetDynamicObjectPos(_id, pos.x, pos.y, pos.z); }
	bool SetRot(Coordinate3D& rot) { return SetDynamicObjectRot(_id, rot.x, rot.y, rot.z); }

	Coordinate3D GetPos() {
		float x, y, z;
		GetDynamicObjectPos(_id, x, y, z);
		return Coordinate3D(x, y, z);
	}

	Coordinate3D GetRot() {
		float x, y, z;
		GetDynamicObjectRot(_id, x, y, z);
		return Coordinate3D(x, y, z);
	}

	int Move(Coordinate3D& pos, float speed, Coordinate3D rot = Coordinate3D(-1000.0, -1000.0, -1000.0)) { return MoveDynamicObject(_id, pos.x, pos.y, pos.z, speed, rot.x, rot.y, rot.z); }
	bool Stop() { return StopDynamicObject(_id); }
	bool IsMoving() { return IsDynamicObjectMoving(_id); }
	bool AttachPlayerCam(int playerid) { return AttachCameraToDynamicObject(playerid, _id); }
	bool AttachToVehicle(int vehicleid, Coordinate3D& offset, Coordinate3D& rot) { return AttachDynamicObjectToVehicle(_id, vehicleid, offset.x, offset.y, offset.z, rot.x, rot.y, rot.z); }
	bool Edit(int playerid) { return EditDynamicObject(playerid, _id); }
};

// Pickup
class DynamicPickup : public EffectiveItem {
public:
	DynamicPickup(int modelid, int type, float x, float y, float z, bool disposable, int worldid = -1, int interiorid = -1, int playerid = -1, float distance = 100.0f)
		: EffectiveItem(CreateDynamicPickup(modelid, type, x, y, z, worldid, interiorid, playerid, distance), disposable) {}
	~DynamicPickup() { DestroyDynamicPickup(_id); }
};

// 3DTextLabel
class Dynamic3DTextLabel {
protected:
	const int _id;

public:
	Dynamic3DTextLabel(const std::string& text, int color, Coordinate5D& pos, float drawdistance, int attachedplayer = INVALID_PLAYER_ID, int attachedvehicle = INVALID_VEHICLE_ID, int testlos = 0, int playerid = -1, float streamdistance = 100.0)
		: _id(CreateDynamic3DTextLabel(text, color, pos.x, pos.y, pos.z, drawdistance, attachedplayer, attachedvehicle, testlos, pos.virtualworld, pos.interior, playerid, streamdistance)) {}
	~Dynamic3DTextLabel() { DestroyDynamic3DTextLabel(_id); }
	bool IsValid() { return IsValidDynamic3DTextLabel(_id); }
	std::string GetText() { std::string text; GetDynamic3DTextLabelText(_id, text); return text; }
	bool SetText(int color, const std::string& text) { return UpdateDynamic3DTextLabelText(_id, color, text); }
};

#endif
