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

#define FCEXPORTIMPL
#include "attribute.h"
#include "exception.h"
#include "logging.h"
#include <boost/lexical_cast.hpp>

AttributeElement::AttributeElement(const std::string& value) : _type(Type::STRING), _value(value) {}

AttributeElement::AttributeElement(const char* value) : _type(Type::STRING), _value(value) {}

AttributeElement::AttributeElement(double value) : _type(Type::NUMBER), _value(boost::lexical_cast<std::string>(value)) {}

AttributeElement::AttributeElement(int value) : _type(Type::NUMBER), _value(boost::lexical_cast<std::string>(value)) {}

AttributeElement::AttributeElement(long value) : _type(Type::NUMBER), _value(boost::lexical_cast<std::string>(value)) {}

AttributeElement::AttributeElement(bool value) : _type(Type::BOOL), _value(boost::lexical_cast<std::string>(value)) {}

AttributeElement::Type AttributeElement::GetValueType() const { 
	return _type;
}

std::string AttributeElement::ToString() const {
	return _value;
}

double AttributeElement::ToNumber() const {
	try {
		return boost::lexical_cast<double>(_value);
	} catch (...) {
		LOGERROR("无法将值 " + ToString() + " 转换为 double");
		throw FCException("数据转换失败");
	}
}

bool AttributeElement::ToBool() const {
	try {
		return boost::lexical_cast<bool>(_value);
	} catch (...) {
		LOGERROR("无法将值 " + ToString() + " 转换为 bool");
		throw FCException("数据转换失败");
	}
}
