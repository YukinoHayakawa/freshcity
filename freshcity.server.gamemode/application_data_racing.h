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

#ifndef FRESHCITY_APPLICATION_DATA_RACING
#define FRESHCITY_APPLICATION_DATA_RACING

#include "application_data_base.h"
#include "application_data_profile.h"
#include <sampgdk/a_samp.h>
#include <sampgdk/a_vehicles.h>

class Racing : public SaveableItem {
public:
	enum Type {
		NORMAL = 0,
		AIR = 1
	};

protected:
	struct RacerInfo {
		int currentcp;
		time_t usedtime;
		int sessionrank;
		RacerInfo() : currentcp(0), usedtime(0), sessionrank(0) {}
	};

	struct RaceData {
		time_t start;
		enum State {
			NONE,
			BUILDING,
			JOINING,
			READY,
			IN_PROGRESS,
			FINISHED
		} state;
		int countdownttimerid;
		int waitingplayer;
		int countdown;
		int starter;
		RaceData() : start(0), state(NONE), countdownttimerid(0), waitingplayer(0), countdown(5), starter(-1) {}
	} _racedata;

	typedef boost::shared_ptr<Coordinate3D> CheckpointPtr;
	typedef std::vector<CheckpointPtr> CheckpointList;
	typedef boost::shared_ptr<RacerInfo> RacerInfoPtr;
	typedef boost::unordered_map<int, RacerInfoPtr> RacerList;
	CheckpointList _checkpoints;
	RacerList _racers;

	struct RaceInfo {
		mongo::OID creator;
		std::string name;
		Type type;
		RaceInfo() : type(NORMAL) {}
	} _raceinfo;

	void _LoadData();

public:
	Racing(const std::string& name, int starter);
	Racing(const std::string& name, int starter, Type type, const mongo::OID& creator);
	~Racing();
	void AppendCheckpoint(const Coordinate3D& pos);
	void Create();
	void Join(Profile& player);
	void Quit(Profile& player);
	bool IsPlayerIn(Profile& player) const;
	void Ready();
	void CountDown();
	void Start();
	void PlayerReachCheckpoint(int playerid);
	void PlayerFinish(int playerid);
	void Finish();
};

#endif
