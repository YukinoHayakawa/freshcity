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
#include "application_data_property.h"
#include "application_gamemode_manager_classes.h"
#include "application_gamemode_item_pickup_classes.h"
#include "basic_algorithm_string.h"

void Property::_LoadData() {
	try {
		std::vector<mongo::BSONElement> xy(_rawdata["xy"].Array());
		_owner		= _rawdata["owner"].OID();
		_name		= UTF8ToGBK(_rawdata["name"].String());
		_value		= (int)_rawdata["value"].Number();
		_profit		= (int)_rawdata["profit"].Number();
		_lastdraw	= (__int64)_rawdata["lastdraw"].Number();
		_x			= (float)xy[0].Number();
		_y			= (float)xy[1].Number();
		_z			= (float)_rawdata["z"].Number();
	} catch(mongo::UserException) {
		throw std::runtime_error("Invalid property data");
	}
}

void Property::_ReloadMarker() {
	if(_pickupid != 0)
		PickupMgr.Remove(_pickupid);
	PickupManager::MemberPtr marker;
	_owner.isSet() ?
		marker.reset(new PropertyMarker_Sold(_uniqueid, _x, _y, _z)) :
		marker.reset(new PropertyMarker_OnSale(_uniqueid, _x, _y, _z));
	_pickupid = marker->GetID();
	PickupMgr.Add(marker);
}

Property::Property(const mongo::BSONObj& data)
	: SaveableItem(CONFIG_STRING("Database.property")) {
		InitData(data);
		_LoadData();
		_ReloadMarker();
}

Property::Property(const std::string& name, const Coordinate3D& pos, int profit, int value)
	: SaveableItem(CONFIG_STRING("Database.property")) {
		mongo::BSONObj submit(BSON(mongo::GENOID <<
			"name"		<< GBKToUTF8(name) <<
			"owner"		<< mongo::OID() <<
			"profit"	<< profit <<
			"value"		<< value <<
			"xy"		<< BSON_ARRAY(pos.x << pos.y) <<
			"z"			<< pos.z <<
			"lastdraw"	<< time(0)));
		SaveableItem::Create(submit, true);
		_LoadData();
		_ReloadMarker();
}

Property::~Property() {
	PickupMgr.Remove(_pickupid);
}

void Property::SetName(const std::string& name) {
	Update(BSON("$set" << BSON("name" << GBKToUTF8(name))), false);
	_name = name;
}

std::string Property::GetName() const {
	return _name;
}

void Property::SetValue(int value) {
	Update(BSON("$set" << BSON("value" << value)), false);
	_value = value;
}

int Property::GetValue() const {
	return _value;
}

void Property::SetProfit(int profit) {
	Update(BSON("$set" << BSON("profit" << profit)), false);
	_profit = profit;
}

int Property::GetProfit() const {
	return _profit;
}

int Property::Draw(Profile& player) {
	if(player.GetUniqueID() != _owner) return 0;
	__int64 now(time(0));
	Update(BSON("$set" << BSON("lastdraw" << now)), false);
	int earns((int)((now - _lastdraw) / 3600) * _profit);
	player.GiveMoney(earns);
	_lastdraw = now;
	return earns;
}

void Property::Purchase(Profile& buyer) {
	if(!OnSale())
		throw std::runtime_error("This property is not on sale");
	if(buyer.GetMoney() < _value)
		throw std::runtime_error("Don't have enough money");
	mongo::OID buyerid(buyer.GetUniqueID());
	Update(BSON("$set" << BSON("owner" << buyerid)), false);
	_owner = buyerid;
	buyer.GiveMoney(_value * -1);
	_ReloadMarker();
}

void Property::Sell(Profile& seller) {
	if(OnSale())
		throw std::runtime_error("This property has already been on sale");
	if(seller.GetUniqueID() != _owner)
		throw std::runtime_error("You aren't the owner");
	Draw(seller);
	mongo::OID empty;
	Update(BSON("$set" << BSON("owner" << empty)), false);
	_owner = empty;
	seller.GiveMoney(_value);
	_ReloadMarker();
}

mongo::OID Property::GetOwner() const {
	return _owner;
}

bool Property::OnSale() const {
	return !_owner.isSet();
}
