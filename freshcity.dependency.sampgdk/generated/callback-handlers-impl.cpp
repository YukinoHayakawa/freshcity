bool OnGameModeInit(AMX *amx, void *callback, cell *retval) {
	typedef bool (PLUGIN_CALL *OnGameModeInitType)();
	((OnGameModeInitType)callback)();
	return true;
}

bool OnGameModeExit(AMX *amx, void *callback, cell *retval) {
	typedef bool (PLUGIN_CALL *OnGameModeExitType)();
	((OnGameModeExitType)callback)();
	return true;
}

bool OnPlayerConnect(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	typedef bool (PLUGIN_CALL *OnPlayerConnectType)(int playerid);
	bool retval_ = ((OnPlayerConnectType)callback)(playerid);
	if (retval != 0) {
		*retval = static_cast<cell>(retval_);
	}
	return (retval_ != false);
}

bool OnPlayerDisconnect(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int reason = sampgdk::Callbacks::GetStackCell(amx, 1);
	typedef bool (PLUGIN_CALL *OnPlayerDisconnectType)(int playerid, int reason);
	bool retval_ = ((OnPlayerDisconnectType)callback)(playerid, reason);
	if (retval != 0) {
		*retval = static_cast<cell>(retval_);
	}
	return (retval_ != false);
}

bool OnPlayerSpawn(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	typedef bool (PLUGIN_CALL *OnPlayerSpawnType)(int playerid);
	bool retval_ = ((OnPlayerSpawnType)callback)(playerid);
	if (retval != 0) {
		*retval = static_cast<cell>(retval_);
	}
	return (retval_ != false);
}

bool OnPlayerDeath(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int killerid = sampgdk::Callbacks::GetStackCell(amx, 1);
	int reason = sampgdk::Callbacks::GetStackCell(amx, 2);
	typedef bool (PLUGIN_CALL *OnPlayerDeathType)(int playerid, int killerid, int reason);
	((OnPlayerDeathType)callback)(playerid, killerid, reason);
	return true;
}

bool OnVehicleSpawn(AMX *amx, void *callback, cell *retval) {
	int vehicleid = sampgdk::Callbacks::GetStackCell(amx, 0);
	typedef bool (PLUGIN_CALL *OnVehicleSpawnType)(int vehicleid);
	((OnVehicleSpawnType)callback)(vehicleid);
	return true;
}

bool OnVehicleDeath(AMX *amx, void *callback, cell *retval) {
	int vehicleid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int killerid = sampgdk::Callbacks::GetStackCell(amx, 1);
	typedef bool (PLUGIN_CALL *OnVehicleDeathType)(int vehicleid, int killerid);
	((OnVehicleDeathType)callback)(vehicleid, killerid);
	return true;
}

bool OnPlayerText(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	std::string text = sampgdk::Callbacks::GetStackString(amx, 1);
	typedef bool (PLUGIN_CALL *OnPlayerTextType)(int playerid, const char * text);
	bool retval_ = ((OnPlayerTextType)callback)(playerid, text.c_str());
	if (retval != 0) {
		*retval = static_cast<cell>(retval_);
	}
	return (retval_ != false);
}

bool OnPlayerCommandText(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	std::string cmdtext = sampgdk::Callbacks::GetStackString(amx, 1);
	typedef bool (PLUGIN_CALL *OnPlayerCommandTextType)(int playerid, const char * cmdtext);
	bool retval_ = ((OnPlayerCommandTextType)callback)(playerid, cmdtext.c_str());
	if (retval != 0) {
		*retval = static_cast<cell>(retval_);
	}
	return (retval_ != true);
}

bool OnPlayerRequestClass(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int classid = sampgdk::Callbacks::GetStackCell(amx, 1);
	typedef bool (PLUGIN_CALL *OnPlayerRequestClassType)(int playerid, int classid);
	bool retval_ = ((OnPlayerRequestClassType)callback)(playerid, classid);
	if (retval != 0) {
		*retval = static_cast<cell>(retval_);
	}
	return (retval_ != false);
}

bool OnPlayerEnterVehicle(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int vehicleid = sampgdk::Callbacks::GetStackCell(amx, 1);
	bool ispassenger = sampgdk::Callbacks::GetStackBool(amx, 2);
	typedef bool (PLUGIN_CALL *OnPlayerEnterVehicleType)(int playerid, int vehicleid, bool ispassenger);
	((OnPlayerEnterVehicleType)callback)(playerid, vehicleid, ispassenger);
	return true;
}

bool OnPlayerExitVehicle(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int vehicleid = sampgdk::Callbacks::GetStackCell(amx, 1);
	typedef bool (PLUGIN_CALL *OnPlayerExitVehicleType)(int playerid, int vehicleid);
	((OnPlayerExitVehicleType)callback)(playerid, vehicleid);
	return true;
}

bool OnPlayerStateChange(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int newstate = sampgdk::Callbacks::GetStackCell(amx, 1);
	int oldstate = sampgdk::Callbacks::GetStackCell(amx, 2);
	typedef bool (PLUGIN_CALL *OnPlayerStateChangeType)(int playerid, int newstate, int oldstate);
	((OnPlayerStateChangeType)callback)(playerid, newstate, oldstate);
	return true;
}

bool OnPlayerEnterCheckpoint(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	typedef bool (PLUGIN_CALL *OnPlayerEnterCheckpointType)(int playerid);
	bool retval_ = ((OnPlayerEnterCheckpointType)callback)(playerid);
	if (retval != 0) {
		*retval = static_cast<cell>(retval_);
	}
	return (retval_ != false);
}

bool OnPlayerLeaveCheckpoint(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	typedef bool (PLUGIN_CALL *OnPlayerLeaveCheckpointType)(int playerid);
	((OnPlayerLeaveCheckpointType)callback)(playerid);
	return true;
}

bool OnPlayerEnterRaceCheckpoint(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	typedef bool (PLUGIN_CALL *OnPlayerEnterRaceCheckpointType)(int playerid);
	bool retval_ = ((OnPlayerEnterRaceCheckpointType)callback)(playerid);
	if (retval != 0) {
		*retval = static_cast<cell>(retval_);
	}
	return (retval_ != false);
}

bool OnPlayerLeaveRaceCheckpoint(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	typedef bool (PLUGIN_CALL *OnPlayerLeaveRaceCheckpointType)(int playerid);
	((OnPlayerLeaveRaceCheckpointType)callback)(playerid);
	return true;
}

bool OnRconCommand(AMX *amx, void *callback, cell *retval) {
	std::string cmd = sampgdk::Callbacks::GetStackString(amx, 0);
	typedef bool (PLUGIN_CALL *OnRconCommandType)(const char * cmd);
	bool retval_ = ((OnRconCommandType)callback)(cmd.c_str());
	if (retval != 0) {
		*retval = static_cast<cell>(retval_);
	}
	return (retval_ != true);
}

bool OnPlayerRequestSpawn(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	typedef bool (PLUGIN_CALL *OnPlayerRequestSpawnType)(int playerid);
	bool retval_ = ((OnPlayerRequestSpawnType)callback)(playerid);
	if (retval != 0) {
		*retval = static_cast<cell>(retval_);
	}
	return (retval_ != false);
}

bool OnObjectMoved(AMX *amx, void *callback, cell *retval) {
	int objectid = sampgdk::Callbacks::GetStackCell(amx, 0);
	typedef bool (PLUGIN_CALL *OnObjectMovedType)(int objectid);
	((OnObjectMovedType)callback)(objectid);
	return true;
}

bool OnPlayerObjectMoved(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int objectid = sampgdk::Callbacks::GetStackCell(amx, 1);
	typedef bool (PLUGIN_CALL *OnPlayerObjectMovedType)(int playerid, int objectid);
	((OnPlayerObjectMovedType)callback)(playerid, objectid);
	return true;
}

bool OnPlayerPickUpPickup(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int pickupid = sampgdk::Callbacks::GetStackCell(amx, 1);
	typedef bool (PLUGIN_CALL *OnPlayerPickUpPickupType)(int playerid, int pickupid);
	((OnPlayerPickUpPickupType)callback)(playerid, pickupid);
	return true;
}

bool OnVehicleMod(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int vehicleid = sampgdk::Callbacks::GetStackCell(amx, 1);
	int componentid = sampgdk::Callbacks::GetStackCell(amx, 2);
	typedef bool (PLUGIN_CALL *OnVehicleModType)(int playerid, int vehicleid, int componentid);
	bool retval_ = ((OnVehicleModType)callback)(playerid, vehicleid, componentid);
	if (retval != 0) {
		*retval = static_cast<cell>(retval_);
	}
	return (retval_ != false);
}

bool OnEnterExitModShop(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int enterexit = sampgdk::Callbacks::GetStackCell(amx, 1);
	int interiorid = sampgdk::Callbacks::GetStackCell(amx, 2);
	typedef bool (PLUGIN_CALL *OnEnterExitModShopType)(int playerid, int enterexit, int interiorid);
	((OnEnterExitModShopType)callback)(playerid, enterexit, interiorid);
	return true;
}

bool OnVehiclePaintjob(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int vehicleid = sampgdk::Callbacks::GetStackCell(amx, 1);
	int paintjobid = sampgdk::Callbacks::GetStackCell(amx, 2);
	typedef bool (PLUGIN_CALL *OnVehiclePaintjobType)(int playerid, int vehicleid, int paintjobid);
	((OnVehiclePaintjobType)callback)(playerid, vehicleid, paintjobid);
	return true;
}

bool OnVehicleRespray(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int vehicleid = sampgdk::Callbacks::GetStackCell(amx, 1);
	int color1 = sampgdk::Callbacks::GetStackCell(amx, 2);
	int color2 = sampgdk::Callbacks::GetStackCell(amx, 3);
	typedef bool (PLUGIN_CALL *OnVehicleResprayType)(int playerid, int vehicleid, int color1, int color2);
	((OnVehicleResprayType)callback)(playerid, vehicleid, color1, color2);
	return true;
}

bool OnVehicleDamageStatusUpdate(AMX *amx, void *callback, cell *retval) {
	int vehicleid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 1);
	typedef bool (PLUGIN_CALL *OnVehicleDamageStatusUpdateType)(int vehicleid, int playerid);
	((OnVehicleDamageStatusUpdateType)callback)(vehicleid, playerid);
	return true;
}

bool OnUnoccupiedVehicleUpdate(AMX *amx, void *callback, cell *retval) {
	int vehicleid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 1);
	int passenger_seat = sampgdk::Callbacks::GetStackCell(amx, 2);
	typedef bool (PLUGIN_CALL *OnUnoccupiedVehicleUpdateType)(int vehicleid, int playerid, int passenger_seat);
	((OnUnoccupiedVehicleUpdateType)callback)(vehicleid, playerid, passenger_seat);
	return true;
}

bool OnPlayerSelectedMenuRow(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int row = sampgdk::Callbacks::GetStackCell(amx, 1);
	typedef bool (PLUGIN_CALL *OnPlayerSelectedMenuRowType)(int playerid, int row);
	((OnPlayerSelectedMenuRowType)callback)(playerid, row);
	return true;
}

bool OnPlayerExitedMenu(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	typedef bool (PLUGIN_CALL *OnPlayerExitedMenuType)(int playerid);
	((OnPlayerExitedMenuType)callback)(playerid);
	return true;
}

bool OnPlayerInteriorChange(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int newinteriorid = sampgdk::Callbacks::GetStackCell(amx, 1);
	int oldinteriorid = sampgdk::Callbacks::GetStackCell(amx, 2);
	typedef bool (PLUGIN_CALL *OnPlayerInteriorChangeType)(int playerid, int newinteriorid, int oldinteriorid);
	((OnPlayerInteriorChangeType)callback)(playerid, newinteriorid, oldinteriorid);
	return true;
}

bool OnPlayerKeyStateChange(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int newkeys = sampgdk::Callbacks::GetStackCell(amx, 1);
	int oldkeys = sampgdk::Callbacks::GetStackCell(amx, 2);
	typedef bool (PLUGIN_CALL *OnPlayerKeyStateChangeType)(int playerid, int newkeys, int oldkeys);
	bool retval_ = ((OnPlayerKeyStateChangeType)callback)(playerid, newkeys, oldkeys);
	if (retval != 0) {
		*retval = static_cast<cell>(retval_);
	}
	return (retval_ != false);
}

bool OnRconLoginAttempt(AMX *amx, void *callback, cell *retval) {
	std::string ip = sampgdk::Callbacks::GetStackString(amx, 0);
	std::string password = sampgdk::Callbacks::GetStackString(amx, 1);
	bool success = sampgdk::Callbacks::GetStackBool(amx, 2);
	typedef bool (PLUGIN_CALL *OnRconLoginAttemptType)(const char * ip, const char * password, bool success);
	((OnRconLoginAttemptType)callback)(ip.c_str(), password.c_str(), success);
	return true;
}

bool OnPlayerUpdate(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	typedef bool (PLUGIN_CALL *OnPlayerUpdateType)(int playerid);
	bool retval_ = ((OnPlayerUpdateType)callback)(playerid);
	if (retval != 0) {
		*retval = static_cast<cell>(retval_);
	}
	return (retval_ != false);
}

bool OnPlayerStreamIn(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int forplayerid = sampgdk::Callbacks::GetStackCell(amx, 1);
	typedef bool (PLUGIN_CALL *OnPlayerStreamInType)(int playerid, int forplayerid);
	((OnPlayerStreamInType)callback)(playerid, forplayerid);
	return true;
}

bool OnPlayerStreamOut(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int forplayerid = sampgdk::Callbacks::GetStackCell(amx, 1);
	typedef bool (PLUGIN_CALL *OnPlayerStreamOutType)(int playerid, int forplayerid);
	((OnPlayerStreamOutType)callback)(playerid, forplayerid);
	return true;
}

bool OnVehicleStreamIn(AMX *amx, void *callback, cell *retval) {
	int vehicleid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int forplayerid = sampgdk::Callbacks::GetStackCell(amx, 1);
	typedef bool (PLUGIN_CALL *OnVehicleStreamInType)(int vehicleid, int forplayerid);
	((OnVehicleStreamInType)callback)(vehicleid, forplayerid);
	return true;
}

bool OnVehicleStreamOut(AMX *amx, void *callback, cell *retval) {
	int vehicleid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int forplayerid = sampgdk::Callbacks::GetStackCell(amx, 1);
	typedef bool (PLUGIN_CALL *OnVehicleStreamOutType)(int vehicleid, int forplayerid);
	((OnVehicleStreamOutType)callback)(vehicleid, forplayerid);
	return true;
}

bool OnDialogResponse(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int dialogid = sampgdk::Callbacks::GetStackCell(amx, 1);
	int response = sampgdk::Callbacks::GetStackCell(amx, 2);
	int listitem = sampgdk::Callbacks::GetStackCell(amx, 3);
	std::string inputtext = sampgdk::Callbacks::GetStackString(amx, 4);
	typedef bool (PLUGIN_CALL *OnDialogResponseType)(int playerid, int dialogid, int response, int listitem, const char * inputtext);
	bool retval_ = ((OnDialogResponseType)callback)(playerid, dialogid, response, listitem, inputtext.c_str());
	if (retval != 0) {
		*retval = static_cast<cell>(retval_);
	}
	return (retval_ != true);
}

bool OnPlayerTakeDamage(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int issuerid = sampgdk::Callbacks::GetStackCell(amx, 1);
	float amount = sampgdk::Callbacks::GetStackFloat(amx, 2);
	int weaponid = sampgdk::Callbacks::GetStackCell(amx, 3);
	typedef bool (PLUGIN_CALL *OnPlayerTakeDamageType)(int playerid, int issuerid, float amount, int weaponid);
	((OnPlayerTakeDamageType)callback)(playerid, issuerid, amount, weaponid);
	return true;
}

bool OnPlayerGiveDamage(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int damagedid = sampgdk::Callbacks::GetStackCell(amx, 1);
	float amount = sampgdk::Callbacks::GetStackFloat(amx, 2);
	int weaponid = sampgdk::Callbacks::GetStackCell(amx, 3);
	typedef bool (PLUGIN_CALL *OnPlayerGiveDamageType)(int playerid, int damagedid, float amount, int weaponid);
	((OnPlayerGiveDamageType)callback)(playerid, damagedid, amount, weaponid);
	return true;
}

bool OnPlayerClickMap(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	float fX = sampgdk::Callbacks::GetStackFloat(amx, 1);
	float fY = sampgdk::Callbacks::GetStackFloat(amx, 2);
	float fZ = sampgdk::Callbacks::GetStackFloat(amx, 3);
	typedef bool (PLUGIN_CALL *OnPlayerClickMapType)(int playerid, float fX, float fY, float fZ);
	((OnPlayerClickMapType)callback)(playerid, fX, fY, fZ);
	return true;
}

bool OnPlayerClickPlayer(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int clickedplayerid = sampgdk::Callbacks::GetStackCell(amx, 1);
	int source = sampgdk::Callbacks::GetStackCell(amx, 2);
	typedef bool (PLUGIN_CALL *OnPlayerClickPlayerType)(int playerid, int clickedplayerid, int source);
	((OnPlayerClickPlayerType)callback)(playerid, clickedplayerid, source);
	return true;
}

bool OnPlayerEditObject(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	bool playerobject = sampgdk::Callbacks::GetStackBool(amx, 1);
	int objectid = sampgdk::Callbacks::GetStackCell(amx, 2);
	int response = sampgdk::Callbacks::GetStackCell(amx, 3);
	float fX = sampgdk::Callbacks::GetStackFloat(amx, 4);
	float fY = sampgdk::Callbacks::GetStackFloat(amx, 5);
	float fZ = sampgdk::Callbacks::GetStackFloat(amx, 6);
	float fRotX = sampgdk::Callbacks::GetStackFloat(amx, 7);
	float fRotY = sampgdk::Callbacks::GetStackFloat(amx, 8);
	float fRotZ = sampgdk::Callbacks::GetStackFloat(amx, 9);
	typedef bool (PLUGIN_CALL *OnPlayerEditObjectType)(int playerid, bool playerobject, int objectid, int response, float fX, float fY, float fZ, float fRotX, float fRotY, float fRotZ);
	((OnPlayerEditObjectType)callback)(playerid, playerobject, objectid, response, fX, fY, fZ, fRotX, fRotY, fRotZ);
	return true;
}

bool OnPlayerEditAttachedObject(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int response = sampgdk::Callbacks::GetStackCell(amx, 1);
	int index = sampgdk::Callbacks::GetStackCell(amx, 2);
	int modelid = sampgdk::Callbacks::GetStackCell(amx, 3);
	int boneid = sampgdk::Callbacks::GetStackCell(amx, 4);
	float fOffsetX = sampgdk::Callbacks::GetStackFloat(amx, 5);
	float fOffsetY = sampgdk::Callbacks::GetStackFloat(amx, 6);
	float fOffsetZ = sampgdk::Callbacks::GetStackFloat(amx, 7);
	float fRotX = sampgdk::Callbacks::GetStackFloat(amx, 8);
	float fRotY = sampgdk::Callbacks::GetStackFloat(amx, 9);
	float fRotZ = sampgdk::Callbacks::GetStackFloat(amx, 10);
	float fScaleX = sampgdk::Callbacks::GetStackFloat(amx, 11);
	float fScaleY = sampgdk::Callbacks::GetStackFloat(amx, 12);
	float fScaleZ = sampgdk::Callbacks::GetStackFloat(amx, 13);
	typedef bool (PLUGIN_CALL *OnPlayerEditAttachedObjectType)(int playerid, int response, int index, int modelid, int boneid, float fOffsetX, float fOffsetY, float fOffsetZ, float fRotX, float fRotY, float fRotZ, float fScaleX, float fScaleY, float fScaleZ);
	((OnPlayerEditAttachedObjectType)callback)(playerid, response, index, modelid, boneid, fOffsetX, fOffsetY, fOffsetZ, fRotX, fRotY, fRotZ, fScaleX, fScaleY, fScaleZ);
	return true;
}

bool OnPlayerSelectObject(AMX *amx, void *callback, cell *retval) {
	int playerid = sampgdk::Callbacks::GetStackCell(amx, 0);
	int type = sampgdk::Callbacks::GetStackCell(amx, 1);
	int objectid = sampgdk::Callbacks::GetStackCell(amx, 2);
	int modelid = sampgdk::Callbacks::GetStackCell(amx, 3);
	float fX = sampgdk::Callbacks::GetStackFloat(amx, 4);
	float fY = sampgdk::Callbacks::GetStackFloat(amx, 5);
	float fZ = sampgdk::Callbacks::GetStackFloat(amx, 6);
	typedef bool (PLUGIN_CALL *OnPlayerSelectObjectType)(int playerid, int type, int objectid, int modelid, float fX, float fY, float fZ);
	((OnPlayerSelectObjectType)callback)(playerid, type, objectid, modelid, fX, fY, fZ);
	return true;
}

void RegisterCallbacks() {
	sampgdk::Callbacks::AddHandler("OnGameModeInit", OnGameModeInit);
	sampgdk::Callbacks::AddHandler("OnGameModeExit", OnGameModeExit);
	sampgdk::Callbacks::AddHandler("OnPlayerConnect", OnPlayerConnect);
	sampgdk::Callbacks::AddHandler("OnPlayerDisconnect", OnPlayerDisconnect);
	sampgdk::Callbacks::AddHandler("OnPlayerSpawn", OnPlayerSpawn);
	sampgdk::Callbacks::AddHandler("OnPlayerDeath", OnPlayerDeath);
	sampgdk::Callbacks::AddHandler("OnVehicleSpawn", OnVehicleSpawn);
	sampgdk::Callbacks::AddHandler("OnVehicleDeath", OnVehicleDeath);
	sampgdk::Callbacks::AddHandler("OnPlayerText", OnPlayerText);
	sampgdk::Callbacks::AddHandler("OnPlayerCommandText", OnPlayerCommandText);
	sampgdk::Callbacks::AddHandler("OnPlayerRequestClass", OnPlayerRequestClass);
	sampgdk::Callbacks::AddHandler("OnPlayerEnterVehicle", OnPlayerEnterVehicle);
	sampgdk::Callbacks::AddHandler("OnPlayerExitVehicle", OnPlayerExitVehicle);
	sampgdk::Callbacks::AddHandler("OnPlayerStateChange", OnPlayerStateChange);
	sampgdk::Callbacks::AddHandler("OnPlayerEnterCheckpoint", OnPlayerEnterCheckpoint);
	sampgdk::Callbacks::AddHandler("OnPlayerLeaveCheckpoint", OnPlayerLeaveCheckpoint);
	sampgdk::Callbacks::AddHandler("OnPlayerEnterRaceCheckpoint", OnPlayerEnterRaceCheckpoint);
	sampgdk::Callbacks::AddHandler("OnPlayerLeaveRaceCheckpoint", OnPlayerLeaveRaceCheckpoint);
	sampgdk::Callbacks::AddHandler("OnRconCommand", OnRconCommand);
	sampgdk::Callbacks::AddHandler("OnPlayerRequestSpawn", OnPlayerRequestSpawn);
	sampgdk::Callbacks::AddHandler("OnObjectMoved", OnObjectMoved);
	sampgdk::Callbacks::AddHandler("OnPlayerObjectMoved", OnPlayerObjectMoved);
	sampgdk::Callbacks::AddHandler("OnPlayerPickUpPickup", OnPlayerPickUpPickup);
	sampgdk::Callbacks::AddHandler("OnVehicleMod", OnVehicleMod);
	sampgdk::Callbacks::AddHandler("OnEnterExitModShop", OnEnterExitModShop);
	sampgdk::Callbacks::AddHandler("OnVehiclePaintjob", OnVehiclePaintjob);
	sampgdk::Callbacks::AddHandler("OnVehicleRespray", OnVehicleRespray);
	sampgdk::Callbacks::AddHandler("OnVehicleDamageStatusUpdate", OnVehicleDamageStatusUpdate);
	sampgdk::Callbacks::AddHandler("OnUnoccupiedVehicleUpdate", OnUnoccupiedVehicleUpdate);
	sampgdk::Callbacks::AddHandler("OnPlayerSelectedMenuRow", OnPlayerSelectedMenuRow);
	sampgdk::Callbacks::AddHandler("OnPlayerExitedMenu", OnPlayerExitedMenu);
	sampgdk::Callbacks::AddHandler("OnPlayerInteriorChange", OnPlayerInteriorChange);
	sampgdk::Callbacks::AddHandler("OnPlayerKeyStateChange", OnPlayerKeyStateChange);
	sampgdk::Callbacks::AddHandler("OnRconLoginAttempt", OnRconLoginAttempt);
	sampgdk::Callbacks::AddHandler("OnPlayerUpdate", OnPlayerUpdate);
	sampgdk::Callbacks::AddHandler("OnPlayerStreamIn", OnPlayerStreamIn);
	sampgdk::Callbacks::AddHandler("OnPlayerStreamOut", OnPlayerStreamOut);
	sampgdk::Callbacks::AddHandler("OnVehicleStreamIn", OnVehicleStreamIn);
	sampgdk::Callbacks::AddHandler("OnVehicleStreamOut", OnVehicleStreamOut);
	sampgdk::Callbacks::AddHandler("OnDialogResponse", OnDialogResponse);
	sampgdk::Callbacks::AddHandler("OnPlayerTakeDamage", OnPlayerTakeDamage);
	sampgdk::Callbacks::AddHandler("OnPlayerGiveDamage", OnPlayerGiveDamage);
	sampgdk::Callbacks::AddHandler("OnPlayerClickMap", OnPlayerClickMap);
	sampgdk::Callbacks::AddHandler("OnPlayerClickPlayer", OnPlayerClickPlayer);
	sampgdk::Callbacks::AddHandler("OnPlayerEditObject", OnPlayerEditObject);
	sampgdk::Callbacks::AddHandler("OnPlayerEditAttachedObject", OnPlayerEditAttachedObject);
	sampgdk::Callbacks::AddHandler("OnPlayerSelectObject", OnPlayerSelectObject);
}
