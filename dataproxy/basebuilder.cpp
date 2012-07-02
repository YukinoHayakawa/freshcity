#include "common.h"
#define FCEXPORTIMPL
#include "db.h"
#include <time.h>
#include "basebuilder.h"

FCEXPORT std::string CreateObject(const std::string& collection) {
	mongo::BSONObj submit = mongo::BSONObjBuilder().genOID().appendNumber("timestamp", time(0)).obj();
	mongo::BSONElement OID;
	submit.getObjectID(OID);
	DBInstance::GetDB().insert(collection, submit);
	return OID.OID().toString();
}
