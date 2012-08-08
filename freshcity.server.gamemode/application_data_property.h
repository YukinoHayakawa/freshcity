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

#ifndef FRESHCITY_APPLICATION_DATA_PROPERTY
#define FRESHCITY_APPLICATION_DATA_PROPERTY

#include "application_data_base.h"
#include "application_gamemode_item_streamed.h"

class Property : public SaveableItem {
protected:
	mongo::OID _owner;
	std::string _name;
	int _value, _profit;
	__int64 _lastdraw;
	int _pickupid;
	float _x, _y, _z;
	void _LoadData();
	void _ReloadMarker();

public:
	Property(const mongo::BSONObj& data);
	Property(const std::string& name, const Coordinate3D& pos, int profit, int value);
	~Property();
	void SetName(const std::string& name);
	std::string GetName() const;
	void SetValue(int value);
	int GetValue() const;
	void SetProfit(int profit);
	int GetProfit() const;
	/* Return profit value */
	int Draw(Profile& player);
	void Purchase(Profile& buyer);
	void Sell(Profile& seller);
	mongo::OID GetOwner() const;
	bool OnSale() const;
};

struct PropertyCreateInfo {
	std::string name;
	int value, profit;
	Coordinate3D pos;
};

#endif
