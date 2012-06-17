#define FCEXPORTIMPL
#include "exception.h"
#include "logging.h"

FCException::FCException(const std::string& message) : _errormsg(message) {
	LOGERROR(message);
};

std::string FCException::GetMessage() {
	return _errormsg;
}
