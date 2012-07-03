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
