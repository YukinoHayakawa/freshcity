#define FCEXPORTIMPL
#include "exception.h"

FCException::FCException(const std::string& message) : _errormsg(message) {};

std::string FCException::GetMessage() {
	return _errormsg;
}
