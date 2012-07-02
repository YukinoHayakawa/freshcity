#define FCEXPORTIMPL
#include "exception.h"

FCException::FCException(const std::string& message) : _errormsg(message) {};

std::string FCException::GetMessage() const {
	return _errormsg;
}
