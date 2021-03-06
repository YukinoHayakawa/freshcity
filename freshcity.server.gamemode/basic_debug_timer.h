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

#ifndef FRESHCITY_BASIC_DEBUG_TIMER
#define FRESHCITY_BASIC_DEBUG_TIMER

#include <Windows.h>
#include <string>

class FunctionRunTime {
private:
	std::string _func;
	__int64 _start;

public:
	FunctionRunTime(const char* funcname);
	~FunctionRunTime();
};

#define RUN_TIME_COUNTER FunctionRunTime __timer__##__FUNCTION__(__FUNCTION__)

#endif
