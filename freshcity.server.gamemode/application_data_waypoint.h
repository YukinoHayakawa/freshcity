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

#ifndef FRESHCITY_APPLICTION_DATA_WAYPOINT
#define FRESHCITY_APPLICTION_DATA_WAYPOINT

#include "application_data_base.h"
#include "application_struct_coordinate.h"
#include <string>

class Waypoint : public SaveableItem {
protected:
	Coordinate5D _waypoint;
	void _LoadData();

public:
	Waypoint(const std::string& title);
	Waypoint(const mongo::OID& id);
	Waypoint(const Coordinate5D& target);
	Coordinate5D Get() const;
	void ApplyToPlayer(int playerid) const;
	void ApplyToVehicle(int vid) const;
	void PerformTeleport(int playerid) const;
	void Create(const std::string& title, const mongo::OID& creator);
};

#endif
