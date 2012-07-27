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

#ifndef FRESHCITY_APPLICATION_DATA_GANGZONE
#define FRESHCITY_APPLICATION_DATA_GANGZONE

#include "application_database.h"
#include "application_data_base.h"
#include "application_data_profile.h"
#include <sampgdk/a_samp.h>

class GangZoneItem : public SaveableItem {
protected:
	boost::shared_ptr<GangZone> _zone;
	int _areaid, _triggerid;
	std::string _name;
	std::string _owner;
	int _color;
	int _endtimerid, _timeouttimerid;

	struct TurfWarInfo {
		bool InWar;
		std::string Attacker;
		int MemberDeath, EnemyKill;
		TurfWarInfo() : InWar(false), MemberDeath(0), EnemyKill(0) {}
	} _warinfo;

private:
	void _LoadOwnerData();
	void _InitArea();

public:
	GangZoneItem(const std::string& name);
	GangZoneItem(const mongo::BSONObj& data);
	~GangZoneItem();
	void SetName(const std::string& name);
	std::string GetName() const;
	void SetOwner(const std::string& owner);
	std::string GetOwner() const;
	GangZone& Get();
	void Redraw();
	bool StartWar(Profile& attacker);
	void CountEnemyKill();
	void CountMemberDeath();
	/* true = Ï®»÷Õß¶áÈ¡µØÅÌ */
	bool EndWar(bool causedbytimeout = false);
	int GetAreaID() const;
	bool InWar() const;
	std::string GetAttacker() const;
	void MemberArrived();
};

#endif
