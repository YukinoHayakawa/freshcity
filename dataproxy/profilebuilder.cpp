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
#include "../common/hash.h"
#include "db.h"
#include "config.h"
#include "profilebuilder.h"

FCEXPORT std::string CreateProfile(const std::string& password) {
	std::string profilecoll = GetConfig().GetAttribute("Database.profile").ToString();
	std::string profileid = CreateObject(profilecoll);
	DBInstance::GetDB().update(profilecoll, BSON("_id" << mongo::OID(profileid)),
		BSON("$set" << BSON("auth.password" << GetPasswordDigest(password))));
	return profileid;
}
