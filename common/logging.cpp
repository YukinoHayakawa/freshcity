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
#include "logging.h"
#include <fstream>
#include <time.h>
#include <boost/date_time/posix_time/posix_time.hpp>

std::string GetLogFilename() {
	std::stringstream filename;
	filename << "freshcity." << time(0) << ".log";
	return filename.str();
}

std::ofstream logfile(GetLogFilename(), std::ios::app);

FCEXPORT void WriteLog(const char* level, const char* file, const char* function, int line, const std::string& message) {
	logfile <<
		"[" << boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time()) << "][" <<
		level << "][" << file << "(" << line << "): " << function << "] " << message << std::endl;
}
