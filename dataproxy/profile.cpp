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

#include "common.h"
#define FCEXPORTIMPL
#include "profile.h"
#include "config.h"
#include "db.h"
#include "basemembsers.h"
#include "../common/exception.h"
#include "../common/logging.h"
#include "../common/hash.h"

void Profile::FlushPasswordHash() {
	try {
		_passwordhash = _members->rawdata.getObjectField("auth").getField("password").String();
	} catch(...) {
		LOGERROR("位于 " + GetConfig().GetAttribute("Database.profile").ToString() + " 的不包含密码的用户资料 " + _uniqueid);
		throw FCException("不包含密码的用户文档");
	}
}

Profile::Profile() : BaseObject(GetConfig().GetAttribute("Database.profile").ToString()) {}

Profile::Profile(const std::string& profileid)
	: BaseObject(GetConfig().GetAttribute("Database.profile").ToString(), profileid) {
		FlushPasswordHash();
}

bool Profile::HasPassword() const {
	return !_passwordhash.empty();
}

bool Profile::CheckPassword(const std::string& input) const {
	if(_passwordhash.empty()) return false;
	return GetPasswordDigest(input) == _passwordhash;
}

void Profile::SetPassword(const std::string& newpassword) {
	GetDB().update(GetConfig().GetAttribute("Database.profile").ToString(), 
		BSON("_id" << mongo::OID(_uniqueid)), BSON("$set" << BSON("auth.password" << GetPasswordDigest(newpassword))));
	Synchronize();
}

void Profile::Synchronize() {
	BaseObject::Synchronize();
	FlushPasswordHash();
}
