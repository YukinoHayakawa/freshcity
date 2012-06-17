#ifndef FRESHCITY_COMMON_CONFIG
#define FRESHCITY_COMMON_CONFIG

#include "export.h"
#include "attribute.h"
#include <string>
#include <boost/scoped_ptr.hpp>

#pragma warning(disable: 4251)

class ConfigFileParser;

class FCEXPORT ConfigFile {
private:
	std::string _filename;
	boost::scoped_ptr<ConfigFileParser> _parser;

public:
	ConfigFile(const std::string& filename);
	AttributeElement GetAttribute(const std::string& path);
	void SetAttribute(const std::string& path, const std::string& value);
	~ConfigFile();
};

#endif
