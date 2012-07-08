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

#include "application_user_auth.h"
#include "basic_algorithm_sha256.h"

const std::string salt1("Y5Nz.MM;K?xtQkd80V@l?p*RF)I8mH.6n>o`,;`Rlh!3c.?#FZdWMuFPOvnQi#Da");
const std::string salt2("~MDf|JN3?/0w%WHWYcK@/ZDv4ci/vF|UIZi# ^?J*k0oh0`Dy8X.].Tagm!#^0Wu");

std::string GetPasswordDigest(const std::string& source) {
	return sha256(salt1 + source + salt2);
}
