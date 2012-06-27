#include "common.h"
#include "../common/exception.h"
#include "../common/logging.h"
#include "../common/config.h"
#include "db.h"
#include "config.h"

mongo::DBClientConnection dbconnection(true);
bool connected = false;

void ConnectDatabase() {
	if(connected) return;
	std::string host(dbconfig.GetAttribute("Database.host").ToString());
	LOGINFO("正在连接到 " + host + " 的 mongodb 服务器");
	try {
		dbconnection.connect(host);
		LOGINFO("已连接");
	} catch(mongo::DBException &e) {
		LOGERROR("连接数据库时发生错误: " + e.toString());
		throw FCException("无法连接至数据库");
	}
}
