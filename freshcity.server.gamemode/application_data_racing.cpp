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
#include "application_data_racing.h"
#include "basic_algorithm_string.h"
#include "application_gamemode_callback_timer.h"
#include "application_data_waypoint.h"
#include "application_gamemode_manager_classes.h"
#include "basic_debug_logging.h"

#define RACER_FOREACH() for(RacerList::const_iterator iter(_racers.begin()); iter != _racers.end(); ++iter)

void Racing::_LoadData() {
	try {
		_raceinfo.name		= UTF8ToGBK(_rawdata["name"].String());
		_raceinfo.creator	= _rawdata["creator"].OID();
		_raceinfo.type		= (Type)(int)_rawdata["type"].Number();
		std::vector<mongo::BSONElement> cps(_rawdata["checkpoint"].Array());
		for(std::vector<mongo::BSONElement>::const_iterator iter(cps.begin()); iter != cps.end(); ++iter) {
			mongo::BSONObj single(iter->Obj());
			_checkpoints.push_back(CheckpointPtr(new Coordinate3D(
				(float)single[0].Number(), (float)single[1].Number(), (float)single[2].Number())));
		}
	} catch(mongo::UserException& e) {
		LOG_DEBUG(e.what());
		throw std::runtime_error("Invalid racing document");
	}
}

Racing::Racing(const std::string& name, int starter)
	: SaveableItem(CONFIG_STRING("Database.racing"), BSON("name" << GBKToUTF8(name))) {
		_LoadData();
		_racedata.state = RaceData::State::JOINING;
		_racedata.starter = starter;
}

Racing::Racing(const std::string& name, int starter, Type type, const mongo::OID& creator)
	: SaveableItem(CONFIG_STRING("Database.racing")) {
		_raceinfo.name = name;
		_raceinfo.creator = creator;
		_raceinfo.type = type;
		_racedata.state = RaceData::State::BUILDING;
		_racedata.starter = starter;
}

Racing::~Racing() {
	if(_racedata.countdownttimerid != 0)
		DestroyTimer(_racedata.countdownttimerid);
	RACER_FOREACH() {
		TogglePlayerControllable(iter->first, true);
		DisablePlayerRaceCheckpoint(iter->first);
		ProfileMgr[iter->first].DelVar("race_joined");
	}
}

void Racing::AppendCheckpoint(const Coordinate3D& pos) {
	if(_racedata.state != RaceData::State::BUILDING)
		throw std::runtime_error("Not in building progress");
	_checkpoints.push_back(CheckpointPtr(new Coordinate3D(pos)));
}

void Racing::Create() {
	if(_racedata.state != RaceData::State::BUILDING)
		throw std::runtime_error("Not in building progress");
	if(_checkpoints.size() < 2)
		throw std::runtime_error("Checkpoints are less than 2");
	std::vector<mongo::BSONObj> cps;
	for(CheckpointList::const_iterator iter(_checkpoints.begin()); iter != _checkpoints.end(); ++iter) {
		cps.push_back(BSON_ARRAY(iter->get()->x << iter->get()->y << iter->get()->z));
	}
	mongo::BSONObj submit(BSON(mongo::GENOID <<
		"name"			<< GBKToUTF8(_raceinfo.name) <<
		"creator"		<< _raceinfo.creator <<
		"type"			<< (int)_raceinfo.type <<
		"checkpoint"	<< cps));
	SaveableItem::Create(submit, false);
	_racedata.state = RaceData::State::JOINING;
}

void Racing::Join(Profile& player) {
	if(_racedata.state != RaceData::State::JOINING)
		throw std::runtime_error("Race is not ready to join");
	if(player.HasVar("race_joined"))
		throw std::runtime_error("You have already been in a race");
	_racers.insert(std::make_pair(player.GetId(), RacerInfoPtr(new RacerInfo())));
	player.SetVar("race_joined", _racedata.starter);
	Coordinate3D& startcp(*_checkpoints[0].get());
	Coordinate3D& nextcp(*_checkpoints[1].get());
	Waypoint startpoint(Coordinate5D(startcp.x, startcp.y, startcp.z, 0, 0));
	player.SetRaceCheckpoint((_raceinfo.type != Type::AIR) ? 0 : 3,
		startcp.x, startcp.y, nextcp.z, nextcp.x, nextcp.y, nextcp.z, 10.0f);
	startpoint.PerformTeleport(player.GetId());
}

void Racing::Quit(Profile& player) {
	RacerList::iterator iter(_racers.find(player.GetId()));
	if(iter == _racers.end())
		throw std::runtime_error("You are not in the race");
	_racers.erase(iter);
	player.DelVar("race_joined");
	player.DisableRaceCheckpoint();
	--_racedata.waitingplayer;
}

bool Racing::IsPlayerIn(Profile& player) const {
	return _racers.find(player.GetId()) != _racers.end();
}

void Racing::Ready() {
	if(_racedata.state != RaceData::State::JOINING)
		throw std::runtime_error("Race is not ready to start");
	RACER_FOREACH() {
		SendClientMessage(iter->first, COLOR_WARN, "比赛开始倒计时");
		TogglePlayerControllable(iter->first, false);
	}
	_racedata.countdownttimerid = CreateTimer(TimerCallback_RaceCountDown, this, 1000, true);
	_racedata.state = RaceData::State::READY;
}

void Racing::CountDown() {
	if(_racedata.state != RaceData::State::READY)
		throw std::runtime_error("Race is not ready to start");
	if(_racedata.countdown == 0) {
		if(_racedata.countdownttimerid != 0)
			DestroyTimer(_racedata.countdownttimerid);
		_racedata.countdownttimerid = 0;
		return Start();
	}
	std::stringstream strstream;
	strstream << "倒计时 " << _racedata.countdown << " 秒";
	std::string str(strstream.str());
	RACER_FOREACH() {
		SendClientMessage(iter->first, COLOR_WARN, str.c_str());
	}
	--_racedata.countdown;
}

void Racing::Start() {
	if(_racedata.state != RaceData::State::READY)
		throw std::runtime_error("Race is not ready to start");
	_racedata.state = RaceData::State::IN_PROGRESS;
	RACER_FOREACH() {
		SendClientMessage(iter->first, COLOR_WARN, "比赛开始");
		PlayerReachCheckpoint(iter->first);
		TogglePlayerControllable(iter->first, true);
	}
	_racedata.start = time(0);
	_racedata.waitingplayer = _racers.size();
}

void Racing::PlayerReachCheckpoint(int playerid) {
	if(_racedata.state != RaceData::State::IN_PROGRESS) return;
	RacerList::iterator iter(_racers.find(playerid));
	if(iter == _racers.end())
		throw std::runtime_error("You are not in the race");
	++iter->second->currentcp;
	if(iter->second->currentcp == _checkpoints.size())
		return PlayerFinish(iter->first);
	Coordinate3D& currentcp(*_checkpoints[iter->second->currentcp].get());
	if(iter->second->currentcp == _checkpoints.size() - 1) { // Finish
		SetPlayerRaceCheckpoint(iter->first, (_raceinfo.type != Type::AIR) ? 1 : 4,
		currentcp.x, currentcp.y, currentcp.z, 0.0f, 0.0f, 0.0f, 10.0f);
	} else {
		Coordinate3D& nextcp(*_checkpoints[iter->second->currentcp + 1].get());
		SetPlayerRaceCheckpoint(iter->first, (_raceinfo.type != Type::AIR) ? 0 : 3,
		currentcp.x, currentcp.y, currentcp.z, nextcp.x, nextcp.y, nextcp.z, 10.0f);
	}
}

void Racing::PlayerFinish(int playerid) {
	if(_racedata.state != RaceData::State::IN_PROGRESS) return;
	RacerList::iterator iter(_racers.find(playerid));
	--_racedata.waitingplayer;
	iter->second->usedtime = time(0) - _racedata.start;
	iter->second->sessionrank = _racers.size() - _racedata.waitingplayer;
	DisablePlayerRaceCheckpoint(playerid);
	std::stringstream str;
	str << "你在 " << iter->second->usedtime << " 秒内完成了比赛 排名 " << iter->second->sessionrank;
	SendClientMessage(playerid, COLOR_SUCC, str.str().c_str());
	if(_racedata.waitingplayer == 0) Finish();
}

void Racing::Finish() {
	if(_racedata.state != RaceData::State::IN_PROGRESS) return;
	RACER_FOREACH() {
		SendClientMessage(iter->first, COLOR_WARN, "比赛结束");
	}
	RacingMgr.Remove(_racedata.starter);
}
