#define FCEXPORTIMPL
#include "attribute.h"
#include "exception.h"
#include <boost/lexical_cast.hpp>

AttributeElement::AttributeElement(const std::string& value) : _value(value) {}

std::string AttributeElement::ToString() {
	return _value;
}

double AttributeElement::ToNumber() {
	try {
		return boost::lexical_cast<double>(_value);
	} catch (...) {
		throw FCException("无法将值 " + _value + " 转换为 double");
	}
}

bool AttributeElement::ToBool() {
	try {
		return boost::lexical_cast<bool>(_value);
	} catch (...) {
		throw FCException("无法将值 " + _value + " 转换为 bool");
	}
}
