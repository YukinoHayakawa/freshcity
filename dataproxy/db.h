#ifndef FRESHCITY_DATAPROXY_DB
#define FRESHCITY_DATAPROXY_DB

#include "common.h"

extern mongo::DBClientConnection dbconnection;

void ConnectDatabase();

#endif
