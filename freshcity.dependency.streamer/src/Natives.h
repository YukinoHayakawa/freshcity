/*
    SA-MP Streamer Plugin v2.6.1
    Copyright ?2012 Incognito

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef STREAMER_EXPORT_IMPL
#define STREAMER_EXPORT __declspec(dllexport)
#else
#define STREAMER_EXPORT __declspec(dllimport)
#endif

#include <sampgdk/a_objects.h>

#define FLOAT_INFINITY 0x7F800000

#define STREAMER_TYPE_OBJECT (0)
#define STREAMER_TYPE_PICKUP (1)
#define STREAMER_TYPE_CP (2)
#define STREAMER_TYPE_RACE_CP (3)
#define STREAMER_TYPE_MAP_ICON (4)
#define STREAMER_TYPE_3D_TEXT_LABEL (5)
#define STREAMER_TYPE_AREA (6)

#define STREAMER_AREA_TYPE_CIRCLE (0)
#define STREAMER_AREA_TYPE_RECTANGLE (1)
#define STREAMER_AREA_TYPE_SPHERE (2)
#define STREAMER_AREA_TYPE_CUBE (3)
#define STREAMER_AREA_TYPE_POLYGON (4)

#define STREAMER_OBJECT_TYPE_GLOBAL (0)
#define STREAMER_OBJECT_TYPE_PLAYER (1)
#define STREAMER_OBJECT_TYPE_DYNAMIC (2)

STREAMER_EXPORT bool Streamer_SetTickRate(int rate);
STREAMER_EXPORT bool Streamer_SetMaxItems(int type, int items);
STREAMER_EXPORT bool Streamer_SetVisibleItems(int type, int items);
STREAMER_EXPORT void Streamer_SetCellDistance(float streamdistance);
STREAMER_EXPORT void Streamer_SetCellSize(float size);
STREAMER_EXPORT void Streamer_ProcessActiveItems();
STREAMER_EXPORT bool Streamer_ToggleIdleUpdate(int playerid, int toggle);
STREAMER_EXPORT bool Streamer_Update(int playerid);
STREAMER_EXPORT bool Streamer_UpdateEx(int playerid, float x, float y, float z, int worldid = -1, int interiorid = -1);
STREAMER_EXPORT int CreateDynamicObject(int modelid, float x, float y, float z, float rx, float ry, float rz, int worldid = -1, int interiorid = -1, int playerid = -1, float streamdistance = 100.0);
STREAMER_EXPORT bool DestroyDynamicObject(int objectid);
STREAMER_EXPORT bool IsValidDynamicObject(int objectid);
STREAMER_EXPORT bool SetDynamicObjectPos(int objectid, float x, float y, float z);
STREAMER_EXPORT bool GetDynamicObjectPos(int objectid, float& x, float& y, float& z);
STREAMER_EXPORT bool SetDynamicObjectRot(int objectid, float rx, float ry, float rz);
STREAMER_EXPORT bool GetDynamicObjectRot(int objectid, float& rx, float& ry, float& rz);
STREAMER_EXPORT int MoveDynamicObject(int objectid, float x, float y, float z, float speed, float rx = -1000.0, float ry = -1000.0, float rz = -1000.0);
STREAMER_EXPORT bool StopDynamicObject(int objectid);
STREAMER_EXPORT bool IsDynamicObjectMoving(int objectid);
STREAMER_EXPORT bool AttachCameraToDynamicObject(int playerid, int objectid);
STREAMER_EXPORT bool AttachDynamicObjectToVehicle(int objectid, int vehicleid, float offsetx, float offsety, float offsetz, float rx, float ry, float rz);
STREAMER_EXPORT bool EditDynamicObject(int playerid, int objectid);
STREAMER_EXPORT bool GetDynamicObjectMaterial(int objectid, int materialindex, int& modelid, std::string& txdname, std::string& texturename, int& materialcolor);
STREAMER_EXPORT bool SetDynamicObjectMaterial(int objectid, int materialindex, int modelid, const std::string& txdname, const std::string& texturename, int materialcolor = 0);
STREAMER_EXPORT bool GetDynamicObjectMaterialText(int objectid, int materialindex, std::string& text, int& materialsize, std::string& fontface, int& fontsize, bool& bold, int& fontcolor, int& backcolor, int& textalignment);
STREAMER_EXPORT bool SetDynamicObjectMaterialText(int objectid, int materialindex, const std::string& text, int materialsize = OBJECT_MATERIAL_SIZE_256x128, const std::string& fontface = "Arial", int fontsize = 24, bool bold = true, int fontcolor = 0xFFFFFFFF, int backcolor = 0, int textalignment = 0);
STREAMER_EXPORT void DestroyAllDynamicObjects();
STREAMER_EXPORT int CountDynamicObjects();
STREAMER_EXPORT int CreateDynamicPickup(int modelid, int type, float x, float y, float z, int worldid = -1, int interiorid = -1, int playerid = -1, float streamdistance = 100.0);
STREAMER_EXPORT bool DestroyDynamicPickup(int pickupid);
STREAMER_EXPORT bool IsValidDynamicPickup(int pickupid);
STREAMER_EXPORT void DestroyAllDynamicPickups();
STREAMER_EXPORT int CountDynamicPickups();
STREAMER_EXPORT int CreateDynamicCP(float x, float y, float z, float size, int worldid = -1, int interiorid = -1, int playerid = -1, float streamdistance = 100.0);
STREAMER_EXPORT bool DestroyDynamicCP(int checkpointid);
STREAMER_EXPORT bool IsValidDynamicCP(int checkpointid);
STREAMER_EXPORT bool TogglePlayerDynamicCP(int playerid, int checkpointid, bool toggle);
STREAMER_EXPORT bool TogglePlayerAllDynamicCPs(int playerid, bool toggle);
STREAMER_EXPORT bool IsPlayerInDynamicCP(int playerid, int checkpointid);
STREAMER_EXPORT int GetPlayerVisibleDynamicCP(int playerid);
STREAMER_EXPORT void DestroyAllDynamicCPs();
STREAMER_EXPORT int CountDynamicCPs();
STREAMER_EXPORT int CreateDynamicRaceCP(int type, float x, float y, float z, float nextx, float nexty, float nextz, float size, int worldid = -1, int interiorid = -1, int playerid = -1, float streamdistance = 100.0);
STREAMER_EXPORT bool DestroyDynamicRaceCP(int checkpointid);
STREAMER_EXPORT bool IsValidDynamicRaceCP(int checkpointid);
STREAMER_EXPORT bool TogglePlayerDynamicRaceCP(int playerid, int checkpointid, bool toggle);
STREAMER_EXPORT bool TogglePlayerAllDynamicRaceCPs(int playerid, bool toggle);
STREAMER_EXPORT bool IsPlayerInDynamicRaceCP(int playerid, int checkpointid);
STREAMER_EXPORT int GetPlayerVisibleDynamicRaceCP(int playerid);
STREAMER_EXPORT void DestroyAllDynamicRaceCPs();
STREAMER_EXPORT int CountDynamicRaceCPs();
STREAMER_EXPORT int CreateDynamicMapIcon(float x, float y, float z, int type, int color, int worldid = -1, int interiorid = -1, int playerid = -1, float streamdistance = 100.0);
STREAMER_EXPORT bool DestroyDynamicMapIcon(int iconid);
STREAMER_EXPORT bool IsValidDynamicMapIcon(int iconid);
STREAMER_EXPORT void DestroyAllDynamicMapIcons();
STREAMER_EXPORT int CountDynamicMapIcons();
STREAMER_EXPORT int CreateDynamic3DTextLabel(const std::string& text, int color, float x, float y, float z, float drawdistance, int attachedplayer = INVALID_PLAYER_ID, int attachedvehicle = INVALID_VEHICLE_ID, int testlos = 0, int worldid = -1, int interiorid = -1, int playerid = -1, float streamdistance = 100.0);
STREAMER_EXPORT bool DestroyDynamic3DTextLabel(int id);
STREAMER_EXPORT bool IsValidDynamic3DTextLabel(int id);
STREAMER_EXPORT bool GetDynamic3DTextLabelText(int id, std::string& text);
STREAMER_EXPORT bool UpdateDynamic3DTextLabelText(int id, int color, const std::string& text);
STREAMER_EXPORT void DestroyAllDynamic3DTextLabels();
STREAMER_EXPORT int CountDynamic3DTextLabels();
STREAMER_EXPORT int CreateDynamicCircle(float x, float y, float size, int worldid = -1, int interiorid = -1, int playerid = -1);
STREAMER_EXPORT int CreateDynamicRectangle(float minx, float miny, float maxx, float maxy, int worldid = -1, int interiorid = -1, int playerid = -1);
STREAMER_EXPORT int CreateDynamicSphere(float x, float y, float z, float size, int worldid = -1, int interiorid = -1, int playerid = -1);
STREAMER_EXPORT int CreateDynamicCube(float minx, float miny, float minz, float maxx, float maxy, float maxz, int worldid = -1, int interiorid = -1, int playerid = -1);
STREAMER_EXPORT bool DestroyDynamicArea(int areaid);
STREAMER_EXPORT bool IsValidDynamicArea(int areaid);
STREAMER_EXPORT bool TogglePlayerDynamicArea(int playerid, int areaid, bool toggle);
STREAMER_EXPORT bool TogglePlayerAllDynamicAreas(int playerid, bool toggle);
STREAMER_EXPORT bool IsPlayerInDynamicArea(int playerid, int areaid);
STREAMER_EXPORT bool IsPlayerInAnyDynamicArea(int playerid);
STREAMER_EXPORT bool IsPointInDynamicArea(int areaid, float x, float y, float z);
STREAMER_EXPORT bool IsPointInAnyDynamicArea(float x, float y, float z);
STREAMER_EXPORT bool AttachDynamicAreaToObject(int areaid, int objectid, int type = STREAMER_OBJECT_TYPE_DYNAMIC, int playerid = INVALID_PLAYER_ID);
STREAMER_EXPORT bool AttachDynamicAreaToPlayer(int areaid, int playerid);
STREAMER_EXPORT bool AttachDynamicAreaToVehicle(int areaid, int vehicleid);
STREAMER_EXPORT void DestroyAllDynamicAreas();
STREAMER_EXPORT int CountDynamicAreas();
STREAMER_EXPORT bool Streamer_OnPlayerConnect(int playerid);
STREAMER_EXPORT bool Streamer_OnPlayerDisconnect(int playerid, int reason);
STREAMER_EXPORT int Streamer_OnPlayerEditObject(int playerid, bool playerobject, int objectid, int response, float fX, float fY, float fZ, float fRotX, float fRotY, float fRotZ);
STREAMER_EXPORT int Streamer_OnPlayerSelectObject(int playerid, int type, int objectid, int modelid, float fX, float fY, float fZ);
STREAMER_EXPORT int Streamer_OnPlayerPickUpPickup(int playerid, int pickupid);
STREAMER_EXPORT int Streamer_OnPlayerEnterCheckpoint(int playerid);
STREAMER_EXPORT int Streamer_OnPlayerLeaveCheckpoint(int playerid);
STREAMER_EXPORT int Streamer_OnPlayerEnterRaceCheckpoint(int playerid);
STREAMER_EXPORT int Streamer_OnPlayerLeaveRaceCheckpoint(int playerid);
STREAMER_EXPORT void Streamer_ProcessTick();
typedef void (*STREAMER_TWOINTCALLBACK)(int A, int B);
typedef void (*STREAMER_ONEINTCALLBACK)(int A);
STREAMER_EXPORT void SetOnPlayerEnterDynamicAreaCallback(STREAMER_TWOINTCALLBACK callback);
STREAMER_EXPORT void SetOnPlayerLeaveDynamicAreaCallback(STREAMER_TWOINTCALLBACK callback);
STREAMER_EXPORT void SetOnDynamicObjectMovedCallback(STREAMER_ONEINTCALLBACK callback);
