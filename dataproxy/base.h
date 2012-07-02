#ifndef FRESHCITY_DATAPROXY_BASE
#define FRESHCITY_DATAPROXY_BASE

#include "../common/export.h"
#include "../common/attribute.h"
#include <string>
#include <boost/shared_ptr.hpp>

#pragma warning(disable: 4251)

class FCEXPORT BaseObject {
private:
	struct Members;
	std::string _collection;
	std::string _uniqueid;
	boost::shared_ptr<BaseObject::Members> _members;

public:
	BaseObject(const std::string& collection, const std::string& uniqueid);
	BaseObject(const std::string& uniqueid);
	AttributeElement GetAttribute(const std::string& key);
	void SetAttribute(const std::string& key, const AttributeElement& value);
	void SetAttributeIncrease(const std::string& key, double value);
	void RemoveAttribute(const std::string& key);
	void Synchronize();
};

#endif
