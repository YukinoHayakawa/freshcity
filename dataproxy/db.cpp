#include "common.h"
#include "../common/exception.h"
#include "../common/logging.h"
#include "../common/config.h"
#include "db.h"

#pragma comment(lib, "mongoclient.lib")

mongo::DBClientConnection connection_(true);
ConfigFile dbconfig("freshcity.ini");

class DBInit {
public:
	DBInit() {
		std::string host(dbconfig.GetAttribute("Database.host").ToString());
		WriteLog("正在连接到 " + host + " 的 mongodb 服务器");
		try {
			connection_.connect(mongo::HostAndPort(host));
			WriteLog("已连接");
		} catch(mongo::DBException &e) {
			throw FCException("连接数据库时发生错误: " + e.toString());
		}
	}
};

DBInit dbinitinstance;

mongo::DBClientConnection &GetDB() {
	return connection_;
}
