#ifndef FRESHCITY_DATAPROXY_DB
#define FRESHCITY_DATAPROXY_DB

#include "common.h"

class DBInstance {
public:
	static mongo::DBClientConnection& GetDB();
};

#endif
