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

#include "application_database.h"
#include "application_config.h"
#include "basic_debug_logging.h"

#pragma comment(lib, "freshcity.dependency.mongoclient.lib")

mongo::DBClientConnection _dbconnection(true);
bool _connected;

mongo::DBClientConnection& GetDB() {
	if(!_connected) {
		std::string host(CONFIG_STRING("Database.host"));
		LOG_INFO("Trying to connect the mongodb server at " << host.c_str());
		try {
			_dbconnection.connect(host);
			_connected = true;
			LOG_INFO("Connected.");
		} catch(mongo::DBException &e) {
			LOG_FATAL("Failed to connect: " << e.toString().c_str());
			throw;
		}
	}
	return _dbconnection;
}
