#include <iostream>
#include "../dataproxy/base.h"

#pragma comment(lib, "common.lib")
#pragma comment(lib, "dataproxy.lib")

int main() {
	BaseObject testobj("4fe953b43b13e5912178c50c");
	testobj.SetAttribute("name", AttributeElement("TennenColl"));
	testobj.SetAttribute("age", AttributeElement((double)1500000000000));
	testobj.Submit();
	return 0;
}
