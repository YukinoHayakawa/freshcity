#include <iostream>
#include "../dataproxy/base.h"
#include "../dataproxy/basebuilder.h"
#include <time.h>
#include <iostream>

#pragma comment(lib, "common.lib")
#pragma comment(lib, "dataproxy.lib")

int main() {
	std::ios::sync_with_stdio(false);

	std::string newid(CreateObject("freshcity.user"));
	std::cout << newid << std::endl;

	BaseObject obj("freshcity.user", newid);
	obj.SetAttribute("name", AttributeElement("TennenColl"));
	obj.SetAttribute("age", AttributeElement(12));
	obj.Synchronize();

	std::cout << obj.GetAttribute("name").ToString() << std::endl;

	obj.SetAttributeIncrease("age", 3);
	obj.Synchronize();
	
	std::cout << obj.GetAttribute("age").ToString() << std::endl;

	getchar();
	return 0;
}
