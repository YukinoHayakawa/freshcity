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

#ifndef FRESHCITY_APPLICATION_DATA_BASE
#define FRESHCITY_APPLICATION_DATA_BASE

#include "application_database.h"

class SingleObject {
protected:
	mongo::BSONObj _rawdata;
	mongo::OID _uniqueid;
	SingleObject(const std::string& collection, const mongo::OID& uniqueid);
	SingleObject(const std::string& collection, const mongo::BSONObj& query);
	SingleObject(const mongo::BSONObj& data);
	void _Flush(const std::string& collection, const mongo::BSONObj& query);
};

#endif
