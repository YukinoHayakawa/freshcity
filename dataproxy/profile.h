#ifndef FRESHCITY_DATAPROXY_PROFILE
#define FRESHCITY_DATAPROXY_PROFILE

#include "../common/export.h"
#include "../common/attribute.h"
#include "base.h"
#include <string>

#pragma warning(disable: 4251)

class FCEXPORT Profile : public BaseObject {
private:
	std::string _passwordhash;
	void FlushPasswordHash();

public:
	Profile(const std::string& profileid);
	bool CheckPassword(const std::string& input) const;
	void SetPassword(const std::string& newpassword);
	void Synchronize();
};

#endif
