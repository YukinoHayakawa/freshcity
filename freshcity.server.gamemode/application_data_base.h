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
#include "application_config.h"

/* The base class for database element. Throws std::runtime_error. */
class SaveableItem {
protected:
	std::string _collection;
	mongo::BSONObj _rawdata;
	mongo::OID _uniqueid;

public:
	SaveableItem(const std::string& collection, const mongo::OID& uniqueid);
	SaveableItem(const std::string& collection, const mongo::BSONObj& query);
	SaveableItem(const std::string& collection);
	void Refetch();
	void InitData(const mongo::BSONObj& data);
	void Create(const mongo::BSONObj& data, bool refetch);
	void Update(const mongo::BSONObj& modifier, bool refetch);
	mongo::OID GetUniqueID() const;
	bool IsEmpty() const;
	void Destroy();
	operator std::string();
};

#endif
