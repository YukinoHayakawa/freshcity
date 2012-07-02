#ifndef FRESHCITY_COMMON_CONFIG
#define FRESHCITY_COMMON_CONFIG

#include "export.h"
#include "attribute.h"
#include <string>
#include <boost/shared_ptr.hpp>

#pragma warning(disable: 4251)

class FCEXPORT ConfigFile {
private:
	struct Parser;
	std::string _filename;
	boost::shared_ptr<ConfigFile::Parser> _parser;

public:
	ConfigFile(const std::string& filename);
	AttributeElement GetAttribute(const std::string& path) const;
	void SetAttribute(const std::string& path, const std::string& value);
	void Reload();
	void Save();
};

#endif
