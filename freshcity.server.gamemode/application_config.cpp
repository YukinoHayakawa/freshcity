/*
 * Copyright 2012 TennenColl
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdexcept>
#include "application_config.h"
#include "basic_debug_logging.h"
#include <boost/property_tree/ini_parser.hpp>

boost::property_tree::ptree _config;

bool inline LoadConfig() {
	ReloadConfig();
	return true;
}

void* configinit((void*)LoadConfig());

void ReloadConfig() {
	try {
		boost::property_tree::ini_parser::read_ini("freshcity.server.config.ini", _config);
	} catch(...) {
		LOG_FATAL("无法打开配置文件 freshcity.server.config.ini");
		throw std::runtime_error("无法打开配置文件");
	}
}

boost::property_tree::ptree& GetConfig() {
	return _config;
}
