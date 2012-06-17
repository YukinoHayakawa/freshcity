#ifndef FRESHCITY_COMMON_LOGGING
#define FRESHCITY_COMMON_LOGGING

#include "export.h"
#include <string>
#include <boost/lexical_cast.hpp>

FCEXPORT void WriteLog(const char* level, const char* file, const char* function, int line, const std::string& message);

#define LOGINFO(message) WriteLog("INFO", __FILE__, __FUNCTION__, __LINE__, message);
#define LOGWARNING(message) WriteLog("WARNING", __FILE__, __FUNCTION__, __LINE__, message);
#define LOGERROR(message) WriteLog("ERROR", __FILE__, __FUNCTION__, __LINE__, message);
#define TOSTR(src) boost::lexical_cast<std::string>(src)

#endif
