#ifndef FRESHCITY_COMMON_ATTRIBUTE
#define FRESHCITY_COMMON_ATTRIBUTE

#include "export.h"
#include <string>

#pragma warning(disable: 4251)

class FCEXPORT AttributeElement {
private:
	const std::string _value;

public:
	AttributeElement(const std::string& value);
	std::string ToString();
	double ToNumber();
	bool ToBool();
};

#endif
