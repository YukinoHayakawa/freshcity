#ifndef FRESHCITY_COMMON_ATTRIBUTE
#define FRESHCITY_COMMON_ATTRIBUTE

#include "export.h"
#include <string>

#pragma warning(disable: 4251)

class FCEXPORT AttributeElement {
public:
	enum Type {
		STRING = 0,
		NUMBER = 1,
		BOOL = 2
	};

private:
	std::string _value;
	AttributeElement::Type _type;

public:
	AttributeElement(const std::string& value);
	AttributeElement(double value);
	AttributeElement(bool value);
	AttributeElement::Type GetValueType();
	std::string ToString();
	double ToNumber();
	bool ToBool();
};

#endif
