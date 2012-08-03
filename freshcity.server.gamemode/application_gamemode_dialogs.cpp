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

#include "application_database.h"
#include "application_gamemode_manager_classes.h"
#include "application_gamemode_role_classes.h"
#include "application_data_waypoint.h"

class _DialogRegister {
public:
	_DialogRegister(int dialogid, const DialogCell& dlginfo) {
		DialogManager::GetInstance().Add(dialogid, dlginfo);
	}
};

#define DIALOG(id, style, caption, btnOK, btnCancle, mustresponse) \
	void Dlg##id(Profile& player, bool response, int listitem, const char* inputtext);\
	_DialogRegister __DlgReg_##id(id, DialogCell(style, caption, btnOK, btnCancle, Dlg##id, mustresponse));\
	void Dlg##id(Profile& player, bool response, int listitem, const char* inputtext)

DIALOG(DIALOG_TEAM_SELECT, DIALOG_STYLE_LIST, "选择阵营", "确定", "", true) {
	std::string id(inputtext, 24);
	mongo::OID ptid(player.GetTeamId());
	if(ptid != mongo::OID(id)) {
		if(player.GetTeamId().isSet())
			TeamMgr[ptid].Quit(player);
		TeamMgr[mongo::OID(id)].Join(player);
	}
	DlgMgr.Show(DIALOG_ROLE_SELECT, "Assault\nMedic\nMechanic\nEngineer", player.GetId());
}

DIALOG(DIALOG_ROLE_SELECT, DIALOG_STYLE_LIST, "选择职业", "确定", "", true) {
	switch(listitem) {
	case 0:
		player.SetRole(Profile::RolePtr(new Assault(player)));
		break;

	case 1:
		player.SetRole(Profile::RolePtr(new Medic(player)));
		break;

	case 2:
		player.SetRole(Profile::RolePtr(new Mechanic(player)));
		break;

	case 3:
		player.SetRole(Profile::RolePtr(new Engineer(player)));
		break;
	}
	player.Spawn();
}

DIALOG(DIALOG_PROFILE_REGISTER, DIALOG_STYLE_PASSWORD, "注册", "注册", "", true) {
	if(inputtext[0] == 0) {
		player.SendChatMessage(COLOR_ERROR, "密码不能为空");
	} else {
		try {
			player.Create(player.GetName(), inputtext);
			player.SendChatMessage(COLOR_SUCC, "注册成功");
			player.SetSignedIn(true);
			return;
		} catch(std::runtime_error& e) {
			player.SendChatMessage(COLOR_ERROR, e.what());
		} catch(...) {
			player.SendChatMessage(COLOR_ERROR, "由于未知原因注册失败, 请联系服务器管理员");
		}
	}
	DlgMgr.Show(DIALOG_PROFILE_REGISTER, "请输入您的密码:", player.GetId());
}

DIALOG(DIALOG_PROFILE_LOGIN, DIALOG_STYLE_PASSWORD, "登录", "登录", "", true) {
	if(!player.AuthPassword(inputtext)) {
		DlgMgr.Show(DIALOG_PROFILE_LOGIN, "请输入您的密码:", player.GetId());
		throw std::runtime_error("密码错误");
	}
	player.SetSignedIn(true);
	player.ApplyDataToPlayer();
	player.SendChatMessage(COLOR_SUCC, "登录成功");
}

DIALOG(DIALOG_GANGZONE_CHOOSETOSPAWN, DIALOG_STYLE_LIST, "选择出生地区", "确定", "", true) {
	int zoneid;
	sscanf(inputtext, "%d", &zoneid);
	Waypoint sp(GangZoneMgr[zoneid].GetSpawnPoint());
	sp.PerformTeleport(player.GetId());
}

DIALOG(DIALOG_GANGZONE_REMOVE, DIALOG_STYLE_LIST, "选择需要删除的 GangZone", "删除", "取消", false) {
	int zoneid;
	sscanf(inputtext, "%d", &zoneid);
	GangZoneMgr[zoneid].Destroy();
	GangZoneMgr.Remove(zoneid);
	player.SendChatMessage(COLOR_SUCC, "指定 GangZone 已永久移除");
}

DIALOG(DIALOG_TELEPORT_MAIN, DIALOG_STYLE_LIST, "传送", "确定", "取消", false) {
	switch(listitem) {
	case 0: /* Create */
		DlgMgr.Show(DIALOG_TELEPORT_CREATE, "为传送点命名", player.GetId());
		break;

	case 1: /* Use */
		DlgMgr.Show(DIALOG_TELEPORT_USE, "传送点的名称", player.GetId());
		break;

	case 2: /* ViewNearby */ {
		CoordinatePlane center(player.GetPlaneCoordinate());
		mongo::BSONObj query(BSON("xy" << BSON("$near" << BSON_ARRAY(center.x << center.y))));
		std::auto_ptr<mongo::DBClientCursor> results(GetDB().query(CONFIG_STRING("Database.waypoint"), query, 16));
		std::stringstream str;
		while(results->more()) {
			mongo::BSONObj item(results->next());
			str << item["_id"].OID() << "\t" << item["title"].String() << "\n";
		}
		DlgMgr.Show(DIALOG_TELEPORT_NEARBY, str.str(), player.GetId());
		break;
	}

	case 3: /* CreateTrigger */
		DlgMgr.Show(DIALOG_TELEPORT_CREATETRIGGER, "传送点的名称", player.GetId());
		break;

	default:
		break;
	}
}

DIALOG(DIALOG_TELEPORT_CREATE, DIALOG_STYLE_INPUT, "创建传送点", "创建", "取消", false) {
	if(inputtext[0] == 0) return DlgMgr.Show(DIALOG_TELEPORT_CREATE, "名称不能为空", player.GetId());
	Waypoint create(player.GetDetailedPos());
	create.Create(inputtext, player.GetUniqueID());
	player.SendChatMessage(COLOR_SUCC, "已创建传送点 " + std::string(inputtext));
}

DIALOG(DIALOG_TELEPORT_USE, DIALOG_STYLE_INPUT, "传送到", "传送", "取消", false) {
	if(inputtext[0] == 0) return DlgMgr.Show(DIALOG_TELEPORT_USE, "名称不能为空", player.GetId());
	Waypoint point(inputtext);
	point.PerformTeleport(player.GetId());
}

DIALOG(DIALOG_TELEPORT_NEARBY, DIALOG_STYLE_LIST, "查看附近的传送点", "传送", "取消", false) {
	Waypoint target(mongo::OID(std::string(inputtext, 24)));
	target.PerformTeleport(player.GetId());
}

DIALOG(DIALOG_TELEPORT_CREATETRIGGER, DIALOG_STYLE_INPUT, "创建传送标记", "创建", "取消", false) {
	Waypoint wp(inputtext);
	CreateTeleportTrigger(wp.GetUniqueID(), player.GetPos());
	player.SendChatMessage(COLOR_SUCC, "已创建到 " + std::string(inputtext) + " 的传送标记");
}
