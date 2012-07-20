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

SaveableItem::SaveableItem(const std::string& collection, const mongo::OID& uniqueid)
	: _collection(collection), _rawdata(GetDB().findOne(_collection, BSON("_id" << uniqueid))),
	_existsindatabase(_rawdata.isEmpty() ? false : true), _uniqueid(!_rawdata.isEmpty() ? _rawdata["_id"].OID() : mongo::OID()) {}

SaveableItem::SaveableItem(const std::string& collection, const mongo::BSONObj& query)
	: _collection(collection), _rawdata(GetDB().findOne(_collection, query)),
	_existsindatabase(_rawdata.isEmpty() ? false : true), _uniqueid(!_rawdata.isEmpty() ? _rawdata["_id"].OID() : mongo::OID()) {}

SaveableItem::SaveableItem(const std::string& collection) : _collection(collection), _existsindatabase(false) {}

void SaveableItem::Refetch() {
	if(!_existsindatabase)
		throw std::runtime_error("尝试重新获取的对象并不存在于数据库中");
	_rawdata = GetDB().findOne(_collection, BSON("_id" << _uniqueid));
	if(_rawdata.isEmpty())
		throw std::runtime_error("数据库返回空文档: " + GetDB().getLastError());
}

void SaveableItem::SetData(const mongo::BSONObj& data) {
	if(_existsindatabase)
		throw std::runtime_error("不能更改非空对象的内容");
	_rawdata = data;
}

void SaveableItem::Create(bool refetch) {
	if(_existsindatabase)
		throw std::runtime_error("尝试保存的对象已经存在与数据库中");
	GetDB().insert(_collection, _rawdata);
	std::string errormsg = GetDB().getLastError();
	if(!errormsg.empty())
		throw std::runtime_error("保存对象失败: " + errormsg);
	mongo::BSONElement OID;
	_rawdata.getObjectID(OID);
	_uniqueid = OID.OID();
	_existsindatabase = true;
	if(refetch) Refetch();
}

void SaveableItem::Update(const mongo::BSONObj& modifier, bool refresh) {
	if(!_existsindatabase)
		throw std::runtime_error("尝试修改的对象并不存在于数据库中");
	GetDB().update(_collection, BSON("_id" << _uniqueid), modifier);
	std::string errormsg = GetDB().getLastError();
	if(!errormsg.empty())
		throw std::runtime_error("更新对象失败: " + errormsg);
	if(refresh) Refetch();
}

bool SaveableItem::IsExistInDatabase() const {
	return _existsindatabase;
}

mongo::OID SaveableItem::GetUniqueID() const {
	return _uniqueid;
}
