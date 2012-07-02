#ifndef FRESHCITY_DATAPROXY_BASEMEMBERS
#define FRESHCITY_DATAPROXY_BASEMEMBERS

#include "common.h"
#include "base.h"
#include <boost/unordered_map.hpp>
#include <vector>

struct BaseObject::Members {
	typedef boost::shared_ptr<AttributeElement> ElementPtr;
	typedef boost::unordered_map<std::string, ElementPtr> ElementBuffer;
	typedef boost::unordered_map<std::string, double> IncreaseBuffer;
	typedef std::vector<std::string> RemoveList;
	ElementBuffer loaded;
	ElementBuffer set;
	IncreaseBuffer increase;
	RemoveList remove;
	mongo::BSONObj rawdata;
};

#endif
