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
	_uniqueid(!_rawdata.isEmpty() ? _rawdata["_id"].OID() : mongo::OID()) {}

SaveableItem::SaveableItem(const std::string& collection, const mongo::BSONObj& query)
	: _collection(collection), _rawdata(GetDB().findOne(_collection, query)),
	_uniqueid(!_rawdata.isEmpty() ? _rawdata["_id"].OID() : mongo::OID()) {}

SaveableItem::SaveableItem(const std::string& collection) : _collection(collection) {}

void SaveableItem::Refetch() {
	if(_uniqueid.compare(mongo::OID()) == 0)
		throw std::runtime_error("尝试重新获取的对象不存在");
	_rawdata = GetDB().findOne(_collection, BSON("_id" << _uniqueid));
	if(_rawdata.isEmpty())
		throw std::runtime_error("数据库返回空文档: " + GetDB().getLastError());
}

void SaveableItem::InitData(const mongo::BSONObj& data) {
	if(!_rawdata.isEmpty())
		throw std::runtime_error("不能更改非空对象的内容");
	_rawdata = data;
	_uniqueid = _rawdata["_id"].OID();
}

void SaveableItem::Create(const mongo::BSONObj& data, bool refetch) {
	if(!_rawdata.isEmpty())
		throw std::runtime_error("尝试保存的对象已经存在");
	GetDB().insert(_collection, data);
	std::string errormsg = GetDB().getLastError();
	if(!errormsg.empty())
		throw std::runtime_error("保存对象失败: " + errormsg);
	mongo::BSONElement OID;
	data.getObjectID(OID);
	_uniqueid = OID.OID();
	if(refetch) Refetch();
}

void SaveableItem::Update(const mongo::BSONObj& modifier, bool refresh) {
	if(_uniqueid.compare(mongo::OID()) == 0)
		throw std::runtime_error("尝试修改的对象不存在");
	GetDB().update(_collection, BSON("_id" << _uniqueid), modifier);
	std::string errormsg = GetDB().getLastError();
	if(!errormsg.empty())
		throw std::runtime_error("更新对象失败: " + errormsg);
	if(refresh) Refetch();
}

mongo::OID SaveableItem::GetUniqueID() const {
	return _uniqueid;
}

bool SaveableItem::IsEmpty() const {
	return _rawdata.isEmpty();
}
