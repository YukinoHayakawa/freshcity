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

#ifndef FRESHCITY_APPLICATION_STRUCT_COORDINATE
#define FRESHCITY_APPLICATION_STRUCT_COORDINATE

struct CoordinatePlane {
	float x, y;
	CoordinatePlane(float x, float y) : x(x), y(y) {}
	CoordinatePlane() : x(0.0f), y(0.0f) {}
};

struct Coordinate3D {
	float x, y, z;
	Coordinate3D(float x, float y, float z) : x(x), y(y), z(z) {}
	Coordinate3D() : x(0.0f), y(0.0f), z(0.0f) {}
};

struct Coordinate5D {
	float x, y, z;
	int virtualworld, interior; 
	Coordinate5D(float x, float y, float z, int world, int interior)
		: x(x), y(y), z(z), virtualworld(world), interior(interior) {}
	Coordinate5D() : x(0.0f), y(0.0f), z(0.0f), virtualworld(0), interior(0) {}
};

#endif
