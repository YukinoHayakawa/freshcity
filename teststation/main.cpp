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

#include <iostream>
#include "../common/minidump.h"
#include "../common/gbkencoder.h"
#include "../dataproxy/profile.h"
#include <time.h>
#include <iostream>

#pragma comment(lib, "common.lib")
#pragma comment(lib, "dataproxy.lib")

int main() {
	SetExceptionHandler();

	std::ios::sync_with_stdio(false);

	Profile profile;
	std::string password;
	std::cout << "Creating a new profile, now set password:" << std::endl;
	std::cin >> password;
	profile.SetPassword(password);

	std::cout << "Verify password:" << std::endl;
	std::cin >> password;
	std::cout << profile.CheckPassword(password) << std::endl;

	std::cout << "Set age and name" << std::endl;
	profile.SetAttribute("name", AttributeElement(GBKToUTF8("É³¶¡Óã")));
	profile.SetAttribute("age", AttributeElement(12));
	std::cout << "Synchronize profile" << std::endl;
	profile.Synchronize();

	std::cout << "name = " << profile.GetAttribute("name").ToString() << std::endl;
	std::cout << "name = " << UTF8ToGBK(profile.GetAttribute("name").ToString()) << std::endl;
	std::cout << "age = " << profile.GetAttribute("age").ToNumber() << std::endl;

	std::cout << "Age += 3" << std::endl;
	profile.SetAttributeIncrease("age", 3);
	std::cout << "Synchronize profile" << std::endl;
	profile.Synchronize();
	
	std::cout << "now age = " << profile.GetAttribute("age").ToString() << std::endl;

	system("pause");
	return 0;
}
