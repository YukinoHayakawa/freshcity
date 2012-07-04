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

#ifndef FRESHCITY_DATAPROXY_BASE
#define FRESHCITY_DATAPROXY_BASE

#include "../common/export.h"
#include "../common/attribute.h"
#include <string>
#include <boost/shared_ptr.hpp>

#pragma warning(disable: 4251)

class FCEXPORT BaseObject {
protected:
	struct Members;
	std::string _collection;
	std::string _uniqueid;
	boost::shared_ptr<BaseObject::Members> _members;

public:
	BaseObject(const std::string& collection, const std::string& uniqueid);
	BaseObject(const std::string& uniqueid);
	AttributeElement GetAttribute(const std::string& key) const;
	void SetAttribute(const std::string& key, const AttributeElement& value);
	void SetAttributeIncrease(const std::string& key, double value);
	void RemoveAttribute(const std::string& key);
	void Synchronize();
};

#endif
