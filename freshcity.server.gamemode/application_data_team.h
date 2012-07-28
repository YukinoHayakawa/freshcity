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

#ifndef FRESHCITY_APPLICATION_DATA_TEAM
#define FRESHCITY_APPLICATION_DATA_TEAM

#include "application_database.h"
#include <boost/unordered_map.hpp>
#include "application_data_profile.h"

class Team : public SaveableItem {
public:
	typedef boost::unordered_map<int, mongo::OID> TeamMemberMap;

protected:
	std::string _name;
	TeamMemberMap _onlineplayers;
	int _ingameteamid;
	int _color;
	mongo::OID _leader;
	void __LoadData();

public:
	Team(const mongo::OID& uniqueid, int teamid);
	Team(const mongo::BSONObj& data, int teamid);
	Team(const mongo::OID& leader, const std::string& name, int color, int teamid);
	void Join(Profile& player);
	void Quit(Profile& player);
	void SetName(const std::string& name);
	std::string GetName() const;
	void SetColor(int color);
	int GetColor() const;
	bool InGame(int playerid) const;
	int GetIngameID() const;
	bool HasOnlineMember() const;
	TeamMemberMap::iterator GetMemberIterator();
	void Create();
	mongo::OID GetLeader();
};

#endif
