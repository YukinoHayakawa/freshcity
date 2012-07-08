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

#include "basic_algorithm_gbkencoder.h"
#include "application_user_auth.h"
#include "application_config.h"
#include <iostream>

int main() {
	std::cout << CONFIG_STRING("Database.host") << std::endl;
	std::cout << GetPasswordDigest("TennenColl") << std::endl;
	std::cout << UTF8ToGBK(GBKToUTF8("É³¶¡Óã")) << std::endl;
	*(int*)0 = 0;
	system("pause");
	return 0;
}
