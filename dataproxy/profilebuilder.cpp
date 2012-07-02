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
