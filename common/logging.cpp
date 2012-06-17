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
