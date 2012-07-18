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
#include "basic_debug_logging.h"

SingleObject::SingleObject(const std::string& collection, const mongo::OID& uniqueid)
	: _collection(collection), _rawdata(GetDB().findOne(_collection, BSON("_id" << uniqueid))),
	_uniqueid(_rawdata.isEmpty() ? mongo::OID() : _rawdata["_id"].OID()), _existsindatabase(_rawdata.isEmpty() ? false : true) {}

SingleObject::SingleObject(const std::string& collection, const mongo::BSONObj& query)
	: _collection(collection), _rawdata(GetDB().findOne(_collection, query)), _uniqueid(_rawdata.isEmpty() ? mongo::OID() : _rawdata["_id"].OID()),
	_existsindatabase(_rawdata.isEmpty() ? false : true) {}

SingleObject::SingleObject(const std::string& collection) : _collection(collection), _existsindatabase(false) {}

void SingleObject::Refresh() {
	if(!_existsindatabase)
		throw std::runtime_error("未指定ID");
	_rawdata = GetDB().findOne(_collection, BSON("_id" << _uniqueid));
	if(_rawdata.isEmpty())
		throw std::runtime_error("刷新文档失败");
}

void SingleObject::Create(const mongo::BSONObj& newdoc, bool refresh) {
	if(_existsindatabase)
		throw std::runtime_error("文档已存在");
	GetDB().insert(_collection, newdoc);
	if(!GetDB().getLastError().empty()) {
		LOG_ERROR(GetDB().getLastError().c_str());
		throw std::runtime_error("插入文档失败");
	}
	mongo::BSONElement OID;
	newdoc.getObjectID(OID);
	_uniqueid = OID.OID();
	_existsindatabase = true;
	if(refresh) Refresh();
}

void SingleObject::Update(const mongo::BSONObj& modifier, bool refresh) {
	if(!_existsindatabase)
		throw std::runtime_error("未指定ID");
	GetDB().update(_collection, BSON("_id" << _uniqueid), modifier);
	if(!GetDB().getLastError().empty()) {
		LOG_ERROR(GetDB().getLastError().c_str());
		throw std::runtime_error("更新文档失败");
	}
	if(refresh) Refresh();
}

void SingleObject::SetCollection(const std::string& collection) {
	_collection = collection;
}

bool SingleObject::IsExistInDatabase() const {
	return _existsindatabase;
}

mongo::OID SingleObject::GetID() const {
	return _uniqueid;
}
