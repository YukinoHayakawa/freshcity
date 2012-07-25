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

#ifndef FRESHCITY_APPLICATION_GAMEMODE_TIMERCALLBACKS
#define FRESHCITY_APPLICATION_GAMEMODE_TIMERCALLBACKS

#include <WinBase.h>

typedef void (CALLBACK*TimerCallbackFunc)(void*, unsigned char);
#define TIMERCALLBACK(x) void CALLBACK TimerCallback_##x(void* param, unsigned char TimerOrWaitFired)
int CreateTimer(TimerCallbackFunc callback, void* param, unsigned long period, bool repeat);
void DestroyTimer(int timerid);

TIMERCALLBACK(EndTurfWar);

#endif
