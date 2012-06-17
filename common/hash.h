#ifndef FRESHCITY_COMMON_HASH
#define FRESHCITY_COMMON_HASH

#include "export.h"
#include <string>

FCEXPORT std::string GetPasswordDigest(const std::string& source);

#endif
