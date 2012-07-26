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
#include "application_config.h"
#include "basic_algorithm_gbkencoder.h"
#include "application_gamemode_manager_classes.h"
#include "application_gamemode_colordefinitions.h"
#include "application_data_dynamicarea_classes.h"
#include "application_data_pickup_classes.h"
#include "application_gamemode_timercallbacks.h"
#include "application_gamemode_sysmsgqueue.h"

void GangZoneItem::_LoadOwnerData() {
	if(!_rawdata.isEmpty()) {
		try {
			_name	= UTF8ToGBK(_rawdata["name"].String());
			_owner	= UTF8ToGBK(_rawdata["owner"].String());
			_color	= TeamManager::GetInstance()[_owner].GetColor() - 0x7F;
			_zone->ShowForAll(_color);
		} catch(mongo::UserException) {
			throw std::runtime_error("无效领地文档");
		}
	} else {
		throw std::runtime_error("不存在的领地文档");
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
	DynamicAreaManager::GetInstance().Add(gzitem);
	PickupManager::MemberPtr trigger(new TurfWarTrigger(_zone->GetId(), triggerx, triggery, triggerz));
	_triggerid = trigger->GetID();
	PickupManager::GetInstance().Add(trigger);
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

GangZoneItem::~GangZoneItem() {
	DynamicAreaManager::GetInstance().Remove(_areaid);
	PickupManager::GetInstance().Remove(_triggerid);
}

void GangZoneItem::SetName(const std::string& name) {
	Update(BSON("$set" << BSON("name" << GBKToUTF8(name))), true);
	_LoadOwnerData();
}

std::string GangZoneItem::GetName() const {
	return _name;
}

void GangZoneItem::SetOwner(const std::string& owner) {
	Update(BSON("$set" << BSON("owner" << GBKToUTF8(owner))), true);
	_LoadOwnerData();
}

std::string GangZoneItem::GetOwner() const {
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
	std::string tname = TeamManager::GetInstance().GetNameByID(attacker.GetTeamFixed());
	if(tname.compare(_owner) == 0) return false;
	SystemMessageQueue::GetInstance().PushMessage(tname + " has started a turfwar");
	_warinfo.Attacker = tname;
	CreateTimer(TimerCallback_EndTurfWar, this, CONFIG_INT("Gaming.turfwarlast") * 60000, false);
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

bool GangZoneItem::EndWar() {
	bool ret = false;
	if(_warinfo.InWar) {
		if(_warinfo.EnemyKill > _warinfo.MemberDeath || _warinfo.MemberDeath == _warinfo.EnemyKill) {
			SystemMessageQueue::GetInstance().PushMessage(GetOwner() + " has won the turfwar");
		} else {
			SystemMessageQueue::GetInstance().PushMessage(_warinfo.Attacker + " has won the turfwar");
			SetOwner(_warinfo.Attacker);
			ret = true;
		}
		_warinfo.EnemyKill = _warinfo.MemberDeath = 0;
		_warinfo.Attacker.clear();
		_warinfo.InWar = false;
		Redraw();
	}
	return ret;
}

int GangZoneItem::GetAreaID() {
	return _areaid;
}

bool GangZoneItem::InWar() {
	return _warinfo.InWar;
}

std::string GangZoneItem::GetAttacker() {
	return _warinfo.Attacker;
}
