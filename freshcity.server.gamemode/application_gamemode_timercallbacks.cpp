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
#include "application_gamemode_timercallbacks.h"
#include "application_gamemode_manager_classes.h"
#include "application_gamemode_colordefinitions.h"

int CreateTimer(TimerCallbackFunc callback, void* param, unsigned long period, bool repeat) {
	HANDLE timerid = 0;
	repeat ?
		CreateTimerQueueTimer(&timerid, 0, (WAITORTIMERCALLBACK)callback, param, period, period, 0) :
		CreateTimerQueueTimer(&timerid, 0, (WAITORTIMERCALLBACK)callback, param, period, 0, 0);
	return (int)timerid;
}

void DestroyTimer(int timerid) {
	DeleteTimerQueueTimer(0, (HANDLE)timerid, 0);
}

#define REF(cls) cls& ref(*(cls*)param)

TIMERCALLBACK(EndTurfWar) {
	REF(GangZoneItem);
	ref.EndWar();
}

TIMERCALLBACK(TurfWarWaitTimeout) {
	REF(GangZoneItem);
	ref.EndWar(true);
}

TIMERCALLBACK(AutoSaveProfile) {
	REF(Profile);
	ref.Sync();
	ref.SendChatMessage(COLOR_SUCC, "Profile synchronized.");
}
