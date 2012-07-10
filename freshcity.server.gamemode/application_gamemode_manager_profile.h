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

#ifndef FRESHCITY_APPLICATION_GAMEMODE_MANAGER_PROFILE
#define FRESHCITY_APPLICATION_GAMEMODE_MANAGER_PROFILE

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include "application_data_profile.h"

class ProfileManager {
protected:
	struct ProfileWithAuthInfo;
	typedef boost::unordered_map<int, ProfileWithAuthInfo> ProfileMap;
	ProfileMap _players;
	ProfileManager();
	~ProfileManager();

public:
	bool Add(int playerid);
	bool IsExist(int playerid) const;
	bool Remove(int playerid);
	Profile& Get(int playerid);
	Profile& operator[](int playerid);
	bool IsAuthed(int playerid) const;
	bool SetAuthed(int playerid, bool authed);
	static ProfileManager& GetInstance();
};

#endif
