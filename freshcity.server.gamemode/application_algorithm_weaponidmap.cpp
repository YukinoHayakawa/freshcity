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

#include "application_algorithm_weaponidmap.h"

int WeaponModelToID[] = {
	0,		331,	333,	334,	335,
	336,	337,	338,	339,	341,
	321,	322,	323,	324,	325,
	326,	342,	343,	344,	0,
	0,		0,		346,	347,	348,
	349,	350,	351,	352,	353,
	355,	356,	372,	357,	358,
	359,	360,	361,	362,	363,
	364,	365,	366,	367,	368,
	369,	371
};

int ConvertWeaponIDToModelID(int weaponid) {
	if(weaponid > (sizeof(WeaponModelToID) / sizeof(int) - 1))
		return 0;
	return WeaponModelToID[weaponid];
}
