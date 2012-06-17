#define FCEXPORTIMPL
#include "../common/exception.h"
#include "../common/logging.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "config.h"

class ConfigFileParser {
public:
	boost::property_tree::ptree _parser;
};

ConfigFile::ConfigFile(const std::string& filename) : _filename(filename), _parser(new ConfigFileParser) {
	try {
		boost::property_tree::ini_parser::read_ini(filename, _parser.get()->_parser);
	} catch(...) {
		throw FCException("无法打开文件 " + filename);
	}
}

AttributeElement ConfigFile::GetAttribute(const std::string& path) {
	try {
		return AttributeElement(_parser->_parser.get<std::string>(path));
	} catch(...) {
		throw FCException("无效路径 " + path);
	}
}

void ConfigFile::SetAttribute(const std::string& path, const std::string& value) {
	try {
		_parser->_parser.get(path, value);
	} catch(...) {
		throw FCException("无效路径 " + path);
	}
}

ConfigFile::~ConfigFile() {
	try {
		boost::property_tree::ini_parser::write_ini(_filename, _parser.get()->_parser);
	} catch(...) {
		LOGERROR("将设置写入到文件 " + _filename + " 时发生错误");
	}
}
