#ifndef FRESHCITY_DATAPROXY_BASE
#define FRESHCITY_DATAPROXY_BASE

#include "../common/export.h"
#include "../common/attribute.h"
#include <string>
#include <boost/shared_ptr.hpp>

#pragma warning(disable: 4251)

class FCEXPORT BaseObject {
private:
	struct AttributeMap;
	std::string _uniqueid;
	std::string _collection;
	boost::shared_ptr<BaseObject::AttributeMap> _attributes;

public:
	BaseObject(const std::string& uniqueid);
	void SetAttribute(const std::string& key, const AttributeElement& value);
	void RemoveAttribute(const std::string& key);
	AttributeElement GetAttribute(const std::string& key);
	void Submit();
};

#endif
