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
#include "application_algorithm_position.h"
#include <math.h>

double inline AngleToRadian(double angle) {
	return (angle / 180) * 3.14;
}

Coordinate3D GenerateDirectionalPoint(Profile& player, float distance) {
	float angle = player.GetFacingAngle();
	Coordinate3D result = player.GetPos();
	if(angle > 0 && angle < 90) {
		result.x -= sin(AngleToRadian(angle)) * distance;
		result.y += cos(AngleToRadian(angle)) * distance;
		return result;
	} else if(angle > 90 && angle < 180) {
		angle -= 90;
		result.x -= cos(AngleToRadian(angle)) * distance;
		result.y -= sin(AngleToRadian(angle)) * distance;
		return result;
	} else if(angle > 180 && angle < 270) {
		angle -= 180;
		result.x += sin(AngleToRadian(angle)) * distance;
		result.y -= cos(AngleToRadian(angle)) * distance;
		return result;
	} else if(angle > 270 && angle < 360) {
		angle -= 270;
		result.x += cos(AngleToRadian(angle)) * distance;
		result.y += sin(AngleToRadian(angle)) * distance;
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
	throw std::runtime_error("无法解析的坐标");
}
