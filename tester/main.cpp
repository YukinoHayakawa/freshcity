#include "../common/config.h"
#include <iostream>

#pragma comment(lib, "common.lib")

int main() {
	ConfigFile test("config.ini");
	std::cout << test.GetAttribute("SectionA.a").ToString() << std::endl;
	std::cout << test.GetAttribute("SectionC.a").ToString() << std::endl;
	return 0;
}
