#define FCEXPORTIMPL
#include "logging.h"
#include <fstream>
#include <time.h>

std::ofstream logfile("freshcity.log", std::ios::app);

FCEXPORT void WriteLog(const std::string& message) {
	logfile << "[" << time(0) << "] " << message << std::endl;
}
