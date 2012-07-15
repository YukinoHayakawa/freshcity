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

#include "application_gamemode_convoy.h"
#include "basic_algorithm_random.h"
#include <sampgdk/a_players.h>
#include <sampgdk/a_vehicles.h>
#include <sampgdk/a_samp.h>

int Convey_CopSkins[] = {285, 288, 289, 267, 266, 223, 165, 46};
int Convey_CriminalsSkins[] = {5, 8, 25, 27, 29, 35, 67, 107, 144, 149, 180, 258};

void Convey_AddPlayerClasses() {
	for(int i = 0; i < sizeof(Convey_CopSkins) / sizeof(int); ++i) //Cops
		AddPlayerClassEx(CONVEY_TEAM_COPS, Convey_CopSkins[i], -2756.76f, 376.11f, 4.34f, 268.19f, 16, 3, 27, 100, 31, 100);
	for(int i = 0; i < sizeof(Convey_CriminalsSkins) / sizeof(int); ++i) //Criminals
		AddPlayerClassEx(CONVEY_TEAM_CRIMINALS, Convey_CriminalsSkins[i], -1686.05f, 1339.01f, 17.25f, 133.40f, 18, 3, 24, 100, 30, 100);
}
