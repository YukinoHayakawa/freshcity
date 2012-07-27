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
#include "application_data_dynamicarea_classes.h"
#include "application_gamemode_manager_classes.h"
#include "application_gamemode_colordefinitions.h"
#include "application_algorithms.h"

// GangZoneArea
GangZoneArea::GangZoneArea(int zoneid, float minx, float miny, float maxx, float maxy)
	: DynamicRectangle(minx, miny, maxx, maxy), _zoneid(zoneid) {}

void GangZoneArea::OnPlayerEnter(Profile& player) {
	GangZoneItem& gz(GangZoneManager::GetInstance()[_zoneid]);
	player.SendChatMessage(COLOR_INFO, "���� " + ColorToEmbeddingString(
		TeamManager::GetInstance()[gz.GetOwner()].GetColor()) + gz.GetName());
	if(gz.InWar() && TeamManager::GetInstance().GetNameByID(player.GetTeamFixed()).compare(gz.GetOwner()) == 0)
		gz.MemberArrived();
}

void GangZoneArea::OnPlayerExit(Profile& player) {
	GangZoneItem& gz(GangZoneManager::GetInstance()[_zoneid]);
	player.SendChatMessage(COLOR_INFO, "�뿪 " + ColorToEmbeddingString(
		TeamManager::GetInstance()[gz.GetOwner()].GetColor()) + gz.GetName());
}
