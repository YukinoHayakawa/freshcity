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
	void _LoadMemberData();
	std::string _nickname;
	std::string _passwordhash;
	mongo::BSONObj _gamearchive;
	int _adminlevel;
	bool _deleted;
	bool _banned;
	bool _signedin;
	int _team;

public:
	Profile(int playerid, const mongo::OID& uniqueid);
	Profile(int playerid, const std::string& logname);
	bool IsProfileDeleted();
	bool IsBannedForGame();
	void ApplyDataToPlayer();
	void Create(const std::string& logname, const std::string& password);
	void Sync();
	bool AuthPassword(const std::string& input) const;
	void SetPassword(const std::string& newpassword);
	CoordinatePlane GetPlaneCoordinate() const;
	std::string GetNickname() const;
	void SetNickname(const std::string& nickname);
	int GetAdminLevel() const;
	void SetAdminLevel(int level);
	Coordinate3D GetCameraFrontVector() const;
	Coordinate3D GetCameraPos() const;
	Coordinate3D GetPos() const;
	Coordinate3D GetVelocity() const;
	void SetBanned(bool banned);
	bool IsSignedIn() const;
	void SetSignedIn(bool signedin);
	Coordinate5D GetDetailedPos() const;
	int GetTeamFixed() const;
	bool SetTeamFixed(int teamid);
};

#endif
