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

#include "basic_debug_timer.h"
#include "basic_debug_logging.h"

LARGE_INTEGER litmp; 
__int64 dfFreq;

bool _DebugTimerInit() {
	QueryPerformanceFrequency(&litmp);
	dfFreq = litmp.QuadPart;
	return true;
}

void* _dbgtimerinit((void*)_DebugTimerInit());

FunctionRunTime::FunctionRunTime(const char* funcname) : _func(funcname) {
	QueryPerformanceCounter(&litmp);
	_start = litmp.QuadPart;
}

FunctionRunTime::~FunctionRunTime() {
	QueryPerformanceCounter(&litmp);
	__int64 _end = litmp.QuadPart;
	__int64 time((_end - _start) / (dfFreq / 1000));
	if(time > 0) LOG_INFO(_func.c_str() << " runs " << time << "ms");
}
