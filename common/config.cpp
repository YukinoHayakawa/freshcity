#define FCEXPORTIMPL
#include "../common/exception.h"
#include "../common/logging.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include "config.h"

struct ConfigFile::Parser {
	boost::property_tree::ptree parser;
};

ConfigFile::ConfigFile(const std::string& filename) : _filename(filename), _parser(new Parser) {
	Reload();
}

AttributeElement ConfigFile::GetAttribute(const std::string& path) const {
	try {
		return AttributeElement(_parser->parser.get<std::string>(path));
	} catch(...) {
		LOGERROR("无效路径 " + path);
		throw FCException("指定路径无效");
	}
}

void ConfigFile::SetAttribute(const std::string& path, const std::string& value) {
	try {
		_parser->parser.get(path, value);
	} catch(...) {
		LOGERROR("无效路径 " + path);
		throw FCException("指定路径无效");
	}
}

void ConfigFile::Reload() {
	try {
		boost::property_tree::ini_parser::read_ini(_filename, _parser.get()->parser);
	} catch(...) {
		LOGERROR("无法打开文件 " + _filename);
		throw FCException("无法打开指定文件");
	}
}

void ConfigFile::Save() {
	try {
		boost::property_tree::ini_parser::write_ini(_filename, _parser.get()->parser);
	} catch(...) {
		LOGERROR("将设置写入到文件 " + _filename + " 时发生错误");
		throw FCException("无法保存指定文件");
	}
}
