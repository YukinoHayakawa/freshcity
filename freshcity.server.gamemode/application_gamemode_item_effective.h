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

#ifndef FRESHCITY_APPLICATION_GAMEMODE_ITEM_EFFECTIVE
#define FRESHCITY_APPLICATION_GAMEMODE_ITEM_EFFECTIVE

#include "application_data_profile.h"

class EffectiveItem {
protected:
	bool _disposable;
	const int _id;

public:
	EffectiveItem(int id, bool disposable) : _id(id), _disposable(disposable) {}
	void virtual Effect(Profile& player) {}
	bool IsDisposable() const { return _disposable; }
	int GetID() const { return _id; }
};

#endif
