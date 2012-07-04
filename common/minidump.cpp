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

#define FCEXPORTIMPL
#include "minidump.h"
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <DbgHelp.h>
#include <sstream>

#pragma comment(lib, "DbgHelp.lib") 

LONG WINAPI ExceptionFilter(struct _EXCEPTION_POINTERS *pExceptionInfo) {
	LONG ret = EXCEPTION_CONTINUE_SEARCH;

	std::wstringstream filename;
	filename << "freshcity." << time(0) << ".dmp";

	HANDLE dumpfile = CreateFile(filename.str().c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (dumpfile != INVALID_HANDLE_VALUE) {
		MINIDUMP_EXCEPTION_INFORMATION exceptioninfo;

		exceptioninfo.ThreadId = GetCurrentThreadId();
		exceptioninfo.ExceptionPointers = pExceptionInfo;
		exceptioninfo.ClientPointers = NULL;

		BOOL bOK = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), dumpfile, MiniDumpNormal, &exceptioninfo, NULL, NULL);
		ret = EXCEPTION_EXECUTE_HANDLER;

		CloseHandle(dumpfile);
	}

	return ret;
}

FCEXPORT void SetExceptionHandler() {
	SetUnhandledExceptionFilter(ExceptionFilter);
}
