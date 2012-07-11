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
#include "basic_algorithm_gbkencoder.h"

#pragma warning(disable: 4244)

void Profile::_LoadMemberData() {
	if(_rawdata.isEmpty())
		throw std::runtime_error("玩家没有注册信息");
	try {
		_passwordhash	= _rawdata["auth"]["password"].String();
		_gamearchive	= _rawdata["archive"]["gtasa"].Obj();
		_banned			= _rawdata["auth"]["deleted"].Bool();
		_nickname		= _gamearchive["reginfo"]["nickname"].String();
		_adminlevel		= _gamearchive["mgmtlevel"].Number();
		_deleted		= _gamearchive["banned"].Bool();
		_registered		= true;
	} catch(mongo::UserException &e) {
		LOG_ERROR(e.what());
		throw std::runtime_error("玩家基本数据不完整");
	}
}

void Profile::ApplyDataToPlayer() {
	if(_gamearchive.isEmpty()) _LoadMemberData();
	try {
		mongo::BSONObj geo			= _gamearchive["geo"].Obj();
		mongo::BSONObj attribute	= _gamearchive["attribute"].Obj();
		std::vector<mongo::BSONElement> coordinate
									= geo["coordinate"].Array();
		std::vector<mongo::BSONElement> weapons
									= attribute["weapon"].Array();

		Spawn();
		SetHealth(attribute["health"].Number());
		SetArmour(attribute["armour"].Number());
		for(std::vector<mongo::BSONElement>::iterator iter = weapons.begin(); iter != weapons.end(); iter++) {
			mongo::BSONObj single = iter->Obj();
			GiveWeapon(single["id"].Number(), single["ammo"].Number());
		}
		SetSkin(attribute["skin"].Number());
		SetColor(attribute["color"].Number());
		SetFightingStyle(attribute["fightingstyle"].Number());
		SetPos(coordinate[0].Number(), coordinate[1].Number(), geo["height"].Number());
		SetFacingAngle(geo["direction"].Number());
		SetInterior(geo["interior"].Number());
		SetVirtualWorld(geo["world"].Number());
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
	: SingleObject(CONFIG_STRING("Database.profile"), uniqueid), Player(playerid),
	_adminlevel(0), _registered(false), _deleted(false), _banned(false) {
		_LoadMemberData();
}

Profile::Profile(int playerid, const std::string& logname)
	: SingleObject(CONFIG_STRING("Database.profile"), BSON("auth.logname" << GBKToUTF8(logname))),
	Player(playerid), _adminlevel(0), _registered(false), _deleted(false), _banned(false) {
		if(!_rawdata.isEmpty())
			_LoadMemberData();
}

Profile::Profile(int playerid, const mongo::BSONObj& data)
	: SingleObject(data), Player(playerid), _adminlevel(0),
	_registered(false), _deleted(false), _banned(false) {
		_LoadMemberData();
}

bool Profile::IsProfileDeleted() {
	return _deleted;
}

bool Profile::IsBannedForGame() {
	return _banned;
}

void Profile::Create(const std::string& logname, const std::string& password) {
	if(IsRegistered())
		throw std::runtime_error("玩家注册资料已存在");
	mongo::BSONObj submit = BSON(mongo::GENOID <<
		"auth"		<< BSON(
			"logname"	<< logname	<<
			"password"	<< GetPasswordDigest(GBKToUTF8(password)) <<
			"ip"		<< GetIp() <<
			"time"		<< mongo::DATENOW <<
			"deleted"	<< false) <<
		"archive"	<< BSON(
			"gtasa"		<< BSON(
				"reginfo"	<< BSON(
					"nickname"	<< GBKToUTF8(logname) <<
					"time"		<< mongo::DATENOW <<
					"ip"		<< GetIp()) <<
				"banned"	<< false <<
				"mgmtlevel"	<< 0)));
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

	mongo::BSONObj submit = BSON(
		"$set" << BSON(
			"archive.gtasa.attribute.health"			<< GetHealth() <<
			"archive.gtasa.attribute.armour"			<< GetArmour() <<
			"archive.gtasa.attribute.weapon"			<< weapons <<
			"archive.gtasa.attribute.skin"				<< GetSkin() <<
			"archive.gtasa.attribute.color"				<< GetColor() <<
			"archive.gtasa.attribute.fightingstyle"		<< GetFightingStyle() <<
			"archive.gtasa.geo.coordinate"				<< BSON_ARRAY(pos.x << pos.y) <<
			"archive.gtasa.geo.height"					<< pos.z <<
			"archive.gtasa.geo.direction"				<< GetFacingAngle() <<
			"archive.gtasa.geo.interior"				<< GetInterior() <<
			"archive.gtasa.geo.world"					<< GetVirtualWorld()
			));
	GetDB().update(CONFIG_STRING("Database.profile"), BSON("_id" << _uniqueid), submit);
	_Flush(CONFIG_STRING("Database.profile"), BSON("_id" << _uniqueid));
	_LoadMemberData();
}

bool Profile::AuthPassword(const std::string& input) const {
	return GetPasswordDigest(GBKToUTF8(input)) == _passwordhash;
}

void Profile::SetPassword(const std::string& newpassword) {
	if(newpassword.empty()) throw std::runtime_error("密码不能为空.");
	_ImmediatelyUpdate(BSON("$set" << BSON("auth.password" << GetPasswordDigest(GBKToUTF8(newpassword)))));
}

bool Profile::IsRegistered() {
	return _registered;
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
	_ImmediatelyUpdate(BSON("$set" << BSON("archive.gtasa.reginfo.nickname" << GBKToUTF8(nickname))));
}

int Profile::GetAdminLevel() const {
	return _adminlevel;
}

void Profile::SetAdminLevel(int level) {
	_ImmediatelyUpdate(BSON("$set" << BSON("archive.gtasa.mgmtlevel" << level)));
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

void Profile::SetBanned(bool banned) {
	_ImmediatelyUpdate(BSON("$set" << BSON("archive.gtasa.banned" << banned)));
}
