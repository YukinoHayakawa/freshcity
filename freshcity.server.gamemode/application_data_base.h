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

/* 用于数据库对象的基类 抛出异常均为 std::runtime_error */
class SaveableItem {
protected:
	/* 对象所在的集合 如: freshcity.profile 建议通过Config获取 */
	std::string _collection;
	/* 对象原始数据 可由 SetData() 更改 */
	mongo::BSONObj _rawdata;
	mongo::OID _uniqueid;

public:
	/* 用于通过ID获取的构造函数 */
	SaveableItem(const std::string& collection, const mongo::OID& uniqueid);
	/* 用于特殊条件查询的构造函数 */
	SaveableItem(const std::string& collection, const mongo::BSONObj& query);
	/* 用于创建新对象(并可能稍后保存)的构造函数 */
	SaveableItem(const std::string& collection);
	/* 重新获取对象内容 若 IsExistInDatabase() == false 则抛出异常 */
	void Refetch();
	/* 设置对象内容 仅在为空对象时才可用 */
	void InitData(const mongo::BSONObj& data);
	/* 将对象内容保存至数据库 若已存在/内容为空/插入失败则抛出异常 可选是否重新获取 */
	void Create(const mongo::BSONObj& data, bool refetch);
	/* 更新对象数据 可选是否重新获取 */
	void Update(const mongo::BSONObj& modifier, bool refetch);
	mongo::OID GetUniqueID() const;
	bool IsEmpty() const;
};

#endif
