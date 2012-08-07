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
#include "application_data_gangzone.h"
#include "basic_algorithm_string.h"
#include "application_gamemode_manager_classes.h"
#include "application_gamemode_item_dynamicarea_classes.h"
#include "application_gamemode_item_pickup_classes.h"
#include "application_gamemode_callback_timer.h"
#include "application_gamemode_sysmsgqueue.h"

void GangZoneItem::_LoadOwnerData() {
	try {
		_name		= UTF8ToGBK(_rawdata["name"].String());
		_owner		= _rawdata["owner"].OID();
		_color		= TeamMgr[_owner].GetColor() - 0x7F;
		_spawnpoint	= _rawdata["spawn"].OID();
		_zone->ShowForAll(_color);
	} catch(mongo::UserException) {
		throw std::runtime_error("Invalid gangzone data");
	}
}

void GangZoneItem::_InitArea() {
	float minx((float)_rawdata["minx"].Number()), miny((float)_rawdata["miny"].Number()),
		maxx((float)_rawdata["maxx"].Number()), maxy((float)_rawdata["maxy"].Number());
	mongo::BSONObj marker(_rawdata["trigger"].Obj());
	float triggerx((float)marker["x"].Number()), triggery((float)marker["y"].Number()),
		triggerz((float)marker["z"].Number());
	_zone.reset(new GangZone(minx, miny, maxx, maxy));
	DynamicAreaManager::MemberPtr gzitem(new GangZoneArea(_zone->GetId(), minx, miny, maxx, maxy));
	_areaid = gzitem->GetID();
	DynAreaMgr.Add(gzitem);
	PickupManager::MemberPtr trigger(new TurfWarTrigger(_zone->GetId(), triggerx, triggery, triggerz));
	_triggerid = trigger->GetID();
	PickupMgr.Add(trigger);
}

GangZoneItem::GangZoneItem(const std::string& name)
	: SaveableItem(CONFIG_STRING("Database.gangzone"), BSON("name" << GBKToUTF8(name))) {
		_InitArea();
		_LoadOwnerData();
}

GangZoneItem::GangZoneItem(const mongo::BSONObj& data)
	: SaveableItem(CONFIG_STRING("Database.gangzone")) {
		InitData(data);
		_InitArea();
		_LoadOwnerData();
}

GangZoneItem::GangZoneItem(const std::string& name, const mongo::OID& owner, const CoordinatePlane& min,
	const CoordinatePlane& max, const mongo::OID& spawnpoint, const Coordinate3D& trigger)
	: SaveableItem(CONFIG_STRING("Database.gangzone")), _zone(new GangZone(min.x, min.y, max.x, max.y)) {
		DynamicAreaManager::MemberPtr gzitem(new GangZoneArea(_zone->GetId(), min.x, min.y, max.x, max.y));
		_areaid = gzitem->GetID();
		DynAreaMgr.Add(gzitem);
		PickupManager::MemberPtr t(new TurfWarTrigger(_zone->GetId(), trigger.x, trigger.y, trigger.z));
		_triggerid = t->GetID();
		PickupMgr.Add(t);
		mongo::BSONObj submit(BSON(mongo::GENOID <<
			"name"		<< GBKToUTF8(name) <<
			"minx"		<< min.x <<
			"miny"		<< min.y <<
			"maxx"		<< max.x <<
			"maxy"		<< max.y <<
			"owner"		<< owner <<
			"spawn"		<< spawnpoint <<
			"trigger"	<< BSON(
				"x"		<< trigger.x <<
				"y"		<< trigger.y <<
				"z"		<< trigger.z)));
		SaveableItem::Create(submit, true);
		_LoadOwnerData();
}

GangZoneItem::~GangZoneItem() {
	DynAreaMgr.Remove(_areaid);
	PickupMgr.Remove(_triggerid);
}

void GangZoneItem::SetName(const std::string& name) {
	Update(BSON("$set" << BSON("name" << GBKToUTF8(name))), true);
	_LoadOwnerData();
}

std::string GangZoneItem::GetName() const {
	return _name;
}

void GangZoneItem::SetOwner(const mongo::OID& owner) {
	Update(BSON("$set" << BSON("owner" << owner)), true);
	_LoadOwnerData();
}

mongo::OID GangZoneItem::GetOwner() const {
	return _owner;
}

GangZone& GangZoneItem::Get() {
	return *_zone.get();
}

void GangZoneItem::Redraw() {
	_warinfo.InWar ? _zone->FlashForAll(COLOR_RED - 0x7F) : _zone->ShowForAll(_color);
}

bool GangZoneItem::StartWar(Profile& attacker) {
	if(_warinfo.InWar) return false;
	if(attacker.GetTeamId() == _owner) return false;
	std::string tname = TeamMgr[attacker.GetTeamId()].GetName();
	SystemMessageQueue::GetInstance().PushMessage(tname + " has started a turfwar");
	_warinfo.Attacker = attacker.GetTeamId();
	_endtimerid = CreateTimer(TimerCallback_EndTurfWar, this,
		CONFIG_INT("Gaming.turfwarlast") * 60000, false);
	bool memberinzone = false;
	for(Team::TeamMemberMap::iterator teammembers(TeamMgr[_owner].GetMemberIterator()), end;
		teammembers != end; ++teammembers) {
		if(GangZoneMgr.GetPointInWhichZone(
			ProfileMgr[teammembers->first].GetPos()) == _zone->GetId()) {
				memberinzone = true;
				break;
		}
	}
	if(!memberinzone)
		_timeouttimerid = CreateTimer(TimerCallback_TurfWarWaitTimeout, this,
		CONFIG_INT("Gaming.turfwartimeout") * 60000, false);
	_warinfo.InWar = true;
	Redraw();
	return true;
}

void GangZoneItem::CountEnemyKill() {
	if(_warinfo.InWar) ++_warinfo.EnemyKill;
}

void GangZoneItem::CountMemberDeath() {
	if(_warinfo.InWar) ++_warinfo.MemberDeath;
}

bool GangZoneItem::EndWar(bool causedbytimeout) {
	bool ret = false;
	if(_warinfo.InWar) {
		if(causedbytimeout) DestroyTimer(_endtimerid);
		if(causedbytimeout || _warinfo.EnemyKill < _warinfo.MemberDeath) {
			SystemMessageQueue::GetInstance().PushMessage(
				TeamMgr[_warinfo.Attacker].GetName() + " has won the turfwar");
			SetOwner(_warinfo.Attacker);
			ret = true;
		} else if(_warinfo.EnemyKill > _warinfo.MemberDeath || _warinfo.MemberDeath == _warinfo.EnemyKill) {
			SystemMessageQueue::GetInstance().PushMessage(
				TeamMgr[GetOwner()].GetName() + " has won the turfwar");
		}
		_warinfo.EnemyKill = _warinfo.MemberDeath = 0;
		_warinfo.Attacker.clear();
		_warinfo.InWar = false;
		Redraw();
	}
	return ret;
}

int GangZoneItem::GetAreaID() const {
	return _areaid;
}

bool GangZoneItem::InWar() const {
	return _warinfo.InWar;
}

mongo::OID GangZoneItem::GetAttacker() const {
	return _warinfo.Attacker;
}

void GangZoneItem::MemberArrived() {
	if(_timeouttimerid != 0) {
		DestroyTimer(_timeouttimerid);
		_timeouttimerid = 0;
	}
}

void GangZoneItem::SetSpawnPoint(const mongo::OID& waypointid) {
	Update(BSON("$set" << BSON("spawn" << waypointid)), true);
	_LoadOwnerData();
}

mongo::OID GangZoneItem::GetSpawnPoint() const {
	return _spawnpoint;
}
