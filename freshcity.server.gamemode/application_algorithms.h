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

#ifndef FRESHCITY_APPLICATION_ALGORITHMS
#define FRESHCITY_APPLICATION_ALGORITHMS

#include "application_data_profile.h"
#include "application_struct_coordinate.h"

// Auth
std::string GetPasswordDigest(const std::string& source);

// Position
Coordinate3D GenerateDirectionalPoint(Profile& player, float distance);

// Converting
double inline AngleToRadian(double angle) {
	return (angle / 180) * 3.14;
}

int ConvertWeaponIDToModelID(int weaponid);

std::string inline ColorToEmbeddingString(int color) {
	char dest[10];
	sprintf(dest, "{%08x", (unsigned long)color);
	dest[7] = '}';
	dest[8] = 0;
	return dest;
}

#endif
