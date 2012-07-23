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
#include "application_gamemode_manager_team.h"
#include "application_gamemode_colordefinitions.h"

void GangZoneItem::_LoadData() {
	if(!_rawdata.isEmpty()) {
		try {
			_name	= UTF8ToGBK(_rawdata["name"].String());
			_owner	= UTF8ToGBK(_rawdata["owner"].String());
			_color	= TeamManager::GetInstance()[_owner].GetColor() - 0x7F;
			_zone.reset(new GangZone(
				(float)_rawdata["minx"].Number(),
				(float)_rawdata["miny"].Number(),
				(float)_rawdata["maxx"].Number(),
				(float)_rawdata["maxy"].Number()));
			_zone->ShowForAll(_color);
		} catch(mongo::UserException) {
			throw std::runtime_error("无效领地文档");
		}
	} else {
		throw std::runtime_error("不存在的领地文档");
	}
}

GangZoneItem::GangZoneItem(const std::string& name)
	: SaveableItem(CONFIG_STRING("Database.gangzone"), BSON("name" << GBKToUTF8(name))) {
		_LoadData();
}

GangZoneItem::GangZoneItem(const mongo::BSONObj& data)
	: SaveableItem(CONFIG_STRING("Database.gangzone")) {
		SetData(data);
		_LoadData();
}

void GangZoneItem::SetName(const std::string& name) {
	Update(BSON("$set" << BSON("name" << GBKToUTF8(name))), true);
	_LoadData();
}

std::string GangZoneItem::GetName() const {
	return _name;
}

void GangZoneItem::SetOwner(const std::string& owner) {
	Update(BSON("$set" << BSON("owner" << GBKToUTF8(owner))), true);
	_LoadData();
}

std::string GangZoneItem::GetOwner() const {
	return _owner;
}

GangZone& GangZoneItem::Get() {
	return *_zone.get();
}

void GangZoneItem::ShowForAll() {
	_zone->ShowForAll(_color);
}
