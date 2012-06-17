#ifndef FRESHCITY_COMMON_EXCEPTION
#define FRESHCITY_COMMON_EXCEPTION

#include "export.h"
#include <string>

#pragma warning(disable: 4251)

class FCEXPORT FCException {
private:
	std::string _errormsg;

public:
	FCException(const std::string& message);
	std::string GetMessage();
};

#endif
