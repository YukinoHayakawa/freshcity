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
#include "../dataproxy/base.h"
#include "../dataproxy/profilebuilder.h"
#include "../dataproxy/profile.h"
#include <time.h>
#include <iostream>

#pragma comment(lib, "common.lib")
#pragma comment(lib, "dataproxy.lib")

int main() {
	SetExceptionHandler();

	std::ios::sync_with_stdio(false);

	std::string newid(CreateProfile("TennenColl"));
	std::cout << newid << std::endl;

	Profile profile(newid);
	profile.SetAttribute("name", AttributeElement("TennenColl"));
	profile.SetAttribute("age", AttributeElement(12));

	std::string password;
	std::cout << "Input password:" << std::endl;
	std::cin >> password;
	std::cout << profile.CheckPassword(password) << std::endl;
	profile.Synchronize();

	std::cout << profile.GetAttribute("name").ToString() << std::endl;
	std::cout << profile.GetAttribute("age").ToNumber() << std::endl;

	profile.SetAttributeIncrease("age", 3);
	profile.Synchronize();
	
	std::cout << profile.GetAttribute("age").ToString() << std::endl;

	profile.SetPassword("freshrat");
	std::cout << "Input password:" << std::endl;
	std::cin >> password;
	std::cout << profile.CheckPassword(password) << std::endl;
	profile.Synchronize();

	*(int*)0 = 0;

	system("pause");
	return 0;
}
