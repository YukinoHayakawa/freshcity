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
#include "application_data_profile.h"
#include "application_config.h"
#include "basic_debug_logging.h"
#include "application_algorithm_auth.h"

#pragma warning(disable: 4244)

void Profile::_FlushMemberData() {
	if(_rawdata.isEmpty())
		throw std::runtime_error("玩家没有注册信息");
	try {
		_passwordhash	= _rawdata["auth"]["password"].String();
		_gamearchive	= _rawdata["archive"]["gtasa"].Obj();

		mongo::BSONObj
			personality	= _gamearchive["personality"].Obj();
		_nickname		= personality["nickname"].String();
		_customcolor	= personality["color"].Number();

		mongo::BSONObj
			admin		= _gamearchive["admin"].Obj();
		_isadmin		= admin["isadmin"].Bool();
		_adminlevel		= admin["level"].Number();
	} catch(mongo::UserException &e) {
		LOG_ERROR(e.what());
		throw std::runtime_error("玩家基本数据不完整");
	}
}

void Profile::_ApplyDataToPlayer() {
	_FlushMemberData();
	try {
		mongo::BSONObj personality	= _gamearchive["personality"].Obj();
		mongo::BSONObj geo			= _gamearchive["geo"].Obj();
		mongo::BSONObj equipment	= _gamearchive["equipment"].Obj();
		mongo::BSONObj score		= _gamearchive["score"].Obj();
		mongo::BSONObj ability		= _gamearchive["ability"].Obj();
		std::vector<mongo::BSONElement> coordinate
									= geo["coordinate"].Array();
		std::vector<mongo::BSONElement> weapons
									= equipment["weapon"].Array();

		Spawn();
		SetSkin(personality["skin"].Number());
		SetColor(_customcolor);
		SetFightingStyle(personality["fightingstyle"].Number());
		SetPos(coordinate[0].Number(), coordinate[1].Number(), geo["height"].Number());
		SetInterior(geo["interior"].Number());
		SetVirtualWorld(geo["world"].Number());
		SetFacingAngle(geo["direction"].Number());
		SetHealth(ability["hp"].Number());
		SetArmour(equipment["armour"].Number());
		for(std::vector<mongo::BSONElement>::iterator iter = weapons.begin(); iter != weapons.end(); iter++) {
			mongo::BSONObj single = iter->Obj();
			GiveWeapon(single["id"].Number(), single["ammo"].Number());
		}
	} catch(mongo::UserException &e) {
		LOG_ERROR(e.what());
		throw std::runtime_error("玩家游戏数据不完整");
	}
}

void inline Profile::_ImmediatelyUpdate(const mongo::BSONObj& modifier) {
	GetDB().update(CONFIG_STRING("Database.profile"), BSON("_id" << _uniqueid), modifier);
	Sync();
}

Profile::Profile(int playerid, const mongo::OID& uniqueid)
	: SingleObject(CONFIG_STRING("Database.profile"), uniqueid), Player(playerid) {
		_ApplyDataToPlayer();
}

Profile::Profile(int playerid, const std::string& logname)
	: SingleObject(CONFIG_STRING("Database.profile"), BSON("auth.logname" << logname)),
	Player(playerid) {
		LOG_TRACE("Profile::Profile(int playerid, const std::string& logname)");
		if(!IsEmpty())
			_ApplyDataToPlayer();
}

Profile::Profile(int playerid, const mongo::BSONObj& data)
	: SingleObject(data), Player(playerid) {
		_ApplyDataToPlayer();
}

void Profile::Create(const std::string& logname, const std::string& password) {
	if(!IsEmpty())
		throw std::runtime_error("玩家注册资料已存在");
	mongo::BSONObj submit = BSON(mongo::GENOID <<
		"auth"		<< BSON(
			"logname"	<< logname	<<
			"password"	<< GetPasswordDigest(password) <<
			"deleted"	<< false) <<
		"jointime"	<< mongo::DATENOW <<
		"archive"	<< BSON(
			"gtasa"		<< BSON(
				"personality"	<< BSON(
					"nickname"	<< logname <<
					"jointime"	<< mongo::DATENOW <<
					"joinip"	<< GetIp() <<
					"color"		<< GetColor()) <<
				"admin"			<< BSON(
					"isadmin"	<< false <<
					"level"		<< 0))));
	mongo::BSONElement OID;
	submit.getObjectID(OID);
	GetDB().insert(CONFIG_STRING("Database.profile"), submit);
	_uniqueid = OID.OID();
	Sync();
}


void Profile::Sync() {
	Coordinate3D pos = GetPos();

	int weapon[13][2];
	std::list<mongo::BSONObj> weapons;
	for(int i = 0; i < 13; i++) {
		GetWeaponData(i, &weapon[i][0], &weapon[i][1]);
		if(weapon[i][0] != 0)
			weapons.push_back(BSON("id" << weapon[i][0] << "ammo" << weapon[i][1]));
	}

	mongo::BSONObj submit = BSON("$set"				<< BSON(
		"archive.gtasa.geo.coordinate"				<< BSON_ARRAY(pos.x << pos.y) <<
		"archive.gtasa.geo.height"					<< pos.z <<
		"archive.gtasa.geo.direction"					<< GetFacingAngle() <<
		"archive.gtasa.geo.world"						<< GetVirtualWorld() <<
		"archive.gtasa.geo.interior"					<< GetInterior() <<
		"archive.gtasa.ability.hp"					<< GetHealth() <<
		"archive.gtasa.personality.skin"			<< GetSkin() <<
		"archive.gtasa.personality.fightingstyle"	<< GetFightingStyle() <<
		"archive.gtasa.equipment.weapon"			<< weapons <<
		"archive.gtasa.equipment.armour"			<< GetArmour()
		));

	GetDB().update(CONFIG_STRING("Database.profile"), BSON("_id" << _uniqueid), submit);
	_Flush(CONFIG_STRING("Database.profile"), BSON("_id" << _uniqueid));
	_FlushMemberData();
}

bool Profile::AuthPassword(const std::string& input) const {
	return GetPasswordDigest(input) == _passwordhash;
}

void Profile::SetPassword(const std::string& newpassword) {
	_ImmediatelyUpdate(BSON("$set" << BSON("auth.password" << GetPasswordDigest(newpassword))));
}

bool Profile::IsEmpty() {
	return _rawdata.isEmpty();
}

CoordinatePlane Profile::GetPlaneCoordinate() const {
	float x, y, z;
	Player::GetPos(x, y, z);
	return CoordinatePlane(x, y);
}

std::string Profile::GetNickname() const {
	return _nickname;
}

void Profile::SetNickname(const std::string& nickname) {
	_ImmediatelyUpdate(BSON("$set" << BSON("archive.gtasa.personality.nickname" << nickname)));
}

int Profile::GetCustomColor() const {
	return _customcolor;
}

void Profile::SetCustomColor(int newcolor) {
	_ImmediatelyUpdate(BSON("$set" << BSON("archive.gtasa.personality.color" << newcolor)));
	SetColor(newcolor);
}

bool Profile::IsAdmin() const {
	return _isadmin;
}

void Profile::SetAdmin(bool isadmin) {
	_ImmediatelyUpdate(BSON("$set" << BSON("archive.gtasa.admin.isadmin" << isadmin)));
}

int Profile::GetAdminLevel() const {
	return _adminlevel;
}

void Profile::SetAdminLevel(int level) {
	_ImmediatelyUpdate(BSON("$set" << BSON("archive.gtasa.admin.level" << level)));
}

Coordinate3D Profile::GetCameraFrontVector() const {
	float x, y, z;
	Player::GetCameraFrontVector(x, y, z);
	return Coordinate3D(x, y, z);
}

Coordinate3D Profile::GetCameraPos() const {
	float x, y, z;
	Player::GetCameraPos(x, y, z);
	return Coordinate3D(x, y, z);
}

Coordinate3D Profile::GetPos() const {
	float x, y, z;
	Player::GetPos(x, y, z);
	return Coordinate3D(x, y, z);
}

Coordinate3D Profile::GetVelocity() const {
	float x, y, z;
	Player::GetVelocity(x, y, z);
	return Coordinate3D(x, y, z);
}
