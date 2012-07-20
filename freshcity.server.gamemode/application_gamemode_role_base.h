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

#ifndef FRESHCITY_APPLICATION_GAMEMODE_ROLE_BASE
#define FRESHCITY_APPLICATION_GAMEMODE_ROLE_BASE

#include <time.h>

class Role {
protected:
	time_t _skilllastuse;
	int _timelimit;
	bool _musthavetarget;

public:
	Role(int timelimit, bool musthavetarget) : _timelimit(timelimit),
		_skilllastuse(0), _musthavetarget(musthavetarget) {}
	void virtual OnSpawn(int playerid) {}
	void virtual PerformSpecialSkill(int targetid) { _skilllastuse = time(0); }
	bool CanPerformSkill() { return (time(0) - _skilllastuse) >= _timelimit; }
	bool MustHaveTarget() { return _musthavetarget; }
};

#endif
