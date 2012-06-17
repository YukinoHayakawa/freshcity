#define FCEXPORTIMPL
#include "hash.h"
#include "sha256.h"

const std::string salt1("Y5Nz.MM;K?xtQkd80V@l?p*RF)I8mH.6n>o`,;`Rlh!3c.?#FZdWMuFPOvnQi#Da");
const std::string salt2("~MDf|JN3?/0w%WHWYcK@/ZDv4ci/vF|UIZi# ^?J*k0oh0`Dy8X.].Tagm!#^0Wu");

FCEXPORT std::string GetPasswordDigest(const std::string& source) {
	return sha256(salt1 + source + salt2);
}
