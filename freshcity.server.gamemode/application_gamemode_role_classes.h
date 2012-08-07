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

#ifndef FRESHCITY_APPLICATION_GAMEMODE_ROLE_CLASSES
#define FRESHCITY_APPLICATION_GAMEMODE_ROLE_CLASSES

#include "application_data_profile.h"
#include "application_gamemode_item_streamed.h"

// Assault
class Assault : public Profile::Role {
public:
	Assault(Profile& player);
	void OnSpawn();
	void PerformSpecialSkill(Profile& target);
};

// Medic
class Medic : public Profile::Role {
public:
	Medic(Profile& player);
	void OnSpawn();
	void PerformSpecialSkill(Profile& target);
};

// Mechanic
class Mechanic : public Profile::Role {
public:
	Mechanic(Profile& player);
	void OnSpawn();
	void PerformSpecialSkill(Profile& target);
};

// Engineer
class Engineer : public Profile::Role {
private:
	boost::shared_ptr<DynamicObject> _obj;

public:
	Engineer(Profile& player);
	void OnSpawn();
	void OnDeath();
	void PerformSpecialSkill(Profile& target);
};

#endif
