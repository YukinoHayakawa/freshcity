#include <iostream>
#include "../dataproxy/base.h"
#include "../dataproxy/profilebuilder.h"
#include "../dataproxy/profile.h"
#include <time.h>
#include <iostream>

#pragma comment(lib, "common.lib")
#pragma comment(lib, "dataproxy.lib")

int main() {
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

	system("pause");
	return 0;
}
