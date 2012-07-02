#include "common.h"
#include "../common/exception.h"
#include "../common/logging.h"
#include "../common/config.h"
#include "db.h"
#include "config.h"
#include "../mongo/client/connpool.h"

mongo::DBClientConnection& DBInstance::GetDB() {
	static mongo::DBClientConnection dbconnection;
	static bool _connected;

	if(!_connected) {
		std::string host(GetConfig().GetAttribute("Database.host").ToString());
		LOGINFO("正在连接到 " + host + " 的 mongodb 服务器");
		try {
			dbconnection.connect(host);
			_connected = true;
			LOGINFO("已连接");
		} catch(mongo::DBException &e) {
			LOGERROR("连接数据库时发生错误: " + e.toString());
			throw FCException("无法连接至数据库");
		}
	}

	return dbconnection;
}
