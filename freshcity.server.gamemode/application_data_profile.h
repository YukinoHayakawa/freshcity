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

#ifndef FRESHCITY_APPLICATION_DATA_PROFILE
#define FRESHCITY_APPLICATION_DATA_PROFILE

#include "application_database.h"
#include "application_data_base.h"
#include "application_struct_coordinate.h"
#include <sampgdk/a_players.h>

class Profile : public SingleObject, public Player {
private:
	void _FlushMemberData();
	void _ApplyDataToPlayer();
	void inline _ImmediatelyUpdate(const mongo::BSONObj& modifier);
	mongo::BSONObj _gamearchive;
	std::string _passwordhash;
	std::string _nickname;
	int _customcolor;
	bool _isadmin;
	int _adminlevel;

public:
	Profile(int playerid, const mongo::OID& uniqueid);
	Profile(int playerid, const std::string& logname);
	Profile(int playerid, const mongo::BSONObj& data);
	void Create(const std::string& logname, const std::string& password);
	void Sync();
	bool AuthPassword(const std::string& input) const;
	void SetPassword(const std::string& newpassword);
	bool IsEmpty();
	CoordinatePlane GetPlaneCoordinate() const;
	std::string GetNickname() const;
	void SetNickname(const std::string& nickname);
	int GetCustomColor() const;
	void SetCustomColor(int newcolor);
	bool IsAdmin() const;
	void SetAdmin(bool isadmin);
	int GetAdminLevel() const;
	void SetAdminLevel(int level);
	Coordinate3D GetCameraFrontVector() const;
	Coordinate3D GetCameraPos() const;
	Coordinate3D GetPos() const;
	Coordinate3D GetVelocity() const;
	//float GetDistanceFromPoint(Coordinate3D target) const;
	//bool SetPos(Coordinate3D target) const;
	//bool SetPosFindZ(Coordinate3D target) const;
	//bool MoveCameraLookAt(Coordinate3D from, Coordinate3D to, int time, int cut = CAMERA_CUT) const;
	//bool MoveCameraPos(Coordinate3D from, Coordinate3D to, int time, int cut = CAMERA_CUT) const;
	//bool IsInRangeOfPoint(float range, Coordinate3D center) const;
	//bool PlayAudioStream(const std::string& url, Coordinate3D center, float distance, bool usepos = false) const;
	//bool RemoveBuilding(int modelid, Coordinate3D center, float radius);
	//bool SetAttachedObject(int index, int modelid, int bone, Coordinate3D offset, Coordinate3D rot, Coordinate3D scale, int color1 = 0, int color2 = 0) const;
	//bool SetCameraLookAt(Coordinate3D target, int cut = CAMERA_CUT) const;
	//bool SetCameraPos(Coordinate3D point) const;
	//bool SetCheckpoint(Coordinate3D point, float size) const;
	//bool SetMapIcon(int iconid, Coordinate3D point, int markertype, int color, int style = MAPICON_LOCAL) const;
};

#endif
