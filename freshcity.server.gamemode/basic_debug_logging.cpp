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

#include "basic_debug_logging.h"
#include "application_config.h"
#include "basic_algorithm_wchar.h"
#include <log4cplus/layout.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/fileappender.h>
#include <memory>

#pragma comment(lib, "freshcity.dependency.log4cplus.lib")

using namespace log4cplus;
using namespace log4cplus::helpers;

Logger globalogger = Logger::getInstance(LOG4CPLUS_TEXT("global"));

bool InitLogger() {
	std::wstringstream filename;
	filename << "freshcity." << time(0) << ".log";

	SharedAppenderPtr fileappender(new FileAppender(filename.str()));
	std::auto_ptr<Layout> filelayout(new PatternLayout(string2wstring(CONFIG_STRING("Logging.filelayout"))));
	fileappender->setLayout(filelayout);

	SharedAppenderPtr consoleappender(new ConsoleAppender());
	std::auto_ptr<Layout> consolelayout(new PatternLayout(string2wstring(CONFIG_STRING("Logging.consolelayout"))));
	consoleappender->setLayout(consolelayout);

	globalogger.addAppender(fileappender);
	globalogger.addAppender(consoleappender);
	globalogger.setLogLevel(ALL_LOG_LEVEL);

	LOG_INFO("Logger has been inited");
	return true;
}

void* loggerinit((void*)InitLogger());
