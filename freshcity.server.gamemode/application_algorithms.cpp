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
#include "application_algorithms.h"
#include "basic_algorithm_string.h"

// Auth
const std::string salt1("Y5Nz.MM;K?xtQkd80V@l?p*RF)I8mH.6n>o`,;`Rlh!3c.?#FZdWMuFPOvnQi#Da");
const std::string salt2("~MDf|JN3?/0w%WHWYcK@/ZDv4ci/vF|UIZi# ^?J*k0oh0`Dy8X.].Tagm!#^0Wu");

std::string GetPasswordDigest(const std::string& source) {
	return sha256(salt1 + source + salt2);
}

// Position
Coordinate3D GenerateDirectionalPoint(Profile& player, float distance) {
	float angle = player.GetFacingAngle();
	Coordinate3D result = player.GetPos();
	if(angle > 0 && angle < 90) {
		result.x -= (float)(sin(AngleToRadian(angle)) * distance);
		result.y += (float)(cos(AngleToRadian(angle)) * distance);
		return result;
	} else if(angle > 90 && angle < 180) {
		angle -= 90;
		result.x -= (float)(cos(AngleToRadian(angle)) * distance);
		result.y -= (float)(sin(AngleToRadian(angle)) * distance);
		return result;
	} else if(angle > 180 && angle < 270) {
		angle -= 180;
		result.x += (float)(sin(AngleToRadian(angle)) * distance);
		result.y -= (float)(cos(AngleToRadian(angle)) * distance);
		return result;
	} else if(angle > 270 && angle < 360) {
		angle -= 270;
		result.x += (float)(cos(AngleToRadian(angle)) * distance);
		result.y += (float)(sin(AngleToRadian(angle)) * distance);
		return result;
	} else if(angle == 0) {
		result.y += distance;
		return result;
	} else if(angle == 90) {
		result.x -= distance;
		return result;
	} else if(angle == 180) {
		result.y -= distance;
		return result;
	} else if(angle == 270) {
		result.x += distance;
		return result;
	} 
	throw std::runtime_error("Incorrect facing angle");
}

// Converting
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
	if(weaponid > (sizeof(WeaponModelToID) / sizeof(int) - 1) || weaponid < 0)
		return 0;
	return WeaponModelToID[weaponid];
}
