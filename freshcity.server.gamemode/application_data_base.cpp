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
#include "application_data_base.h"

SingleObject::SingleObject(const std::string& collection, const mongo::OID& uniqueid)
	: _rawdata(GetDB().findOne(collection, BSON("_id" << uniqueid))), _uniqueid(uniqueid) {}

SingleObject::SingleObject(const std::string& collection, const mongo::BSONObj& query)
	: _rawdata(GetDB().findOne(collection, query)), _uniqueid(_rawdata.isEmpty() ? mongo::OID() : _rawdata["_id"].OID()) {
}

SingleObject::SingleObject(const mongo::BSONObj& data) : _rawdata(data), _uniqueid(_rawdata.hasField("_id") ? _rawdata["_id"].OID() : mongo::OID()) {}

void SingleObject::_Flush(const std::string& collection, const mongo::BSONObj& query) {
	_rawdata = GetDB().findOne(collection, query);
}
