#include "config.h"

ConfigFile dbconfig("freshcity_dataproxy.ini");

ConfigFile& GetConfig() {
	return dbconfig;
}
