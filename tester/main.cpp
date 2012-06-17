#include "../common/logging.h"
#include <iostream>
#include <Windows.h>
#include <sstream>

#pragma comment(lib, "common.lib")
#pragma comment(lib, "dataproxy.lib")

int main() {
	//ConfigFile test("config.ini");
	//std::cout << test.GetAttribute("SectionA.a").ToString() << std::endl;
	//std::cout << test.GetAttribute("SectionC.a").ToString() << std::endl;
	int s, e;
	std::string a("value");
	s = GetTickCount();
	for(int i = 0; i < 10000; ++i)
		LOGINFO("无法将值 " + a + " 转换为 double");
	e = GetTickCount();
	LOGINFO("usedtime: " + TOSTR(e - s) + "ms");
	return 0;
}
