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
#include "application_gamemode_effectiveitem.h"

EffectiveItem::EffectiveItem(int id, bool disposable) : _id(id), _disposable(disposable) {}

void EffectiveItem::Effect(Profile& player) {
	throw std::runtime_error("该物品没有特殊效果");
}

bool EffectiveItem::IsDisposable() {
	return _disposable;
}

int EffectiveItem::GetID() {
	return _id;
}
