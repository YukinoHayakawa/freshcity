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

#ifndef FRESHCITY_APPLICATION_DATA_PROFILE
#define FRESHCITY_APPLICATION_DATA_PROFILE

#include "application_data_base.h"
#include "application_struct_coordinate.h"
#include <sampgdk/a_players.h>
#include <boost/unordered_map.hpp>
#include "application_gamemode_definition_color.h"

class Profile : public SaveableItem, public Player {
public:
	class Role {
	protected:
		time_t _skilllastuse;
		int _timelimit;
		bool _musthavetarget;
		Profile& _player;

	public:
		Role(Profile& player, int timelimit, bool musthavetarget);
		void virtual OnSpawn();
		void virtual OnDeath();
		void virtual PerformSpecialSkill(Profile& target);
		bool CanPerformSkill();
		bool MustHaveTarget();
	};

	typedef boost::shared_ptr<Profile::Role> RolePtr;

private:
	void _LoadMemberData();
	std::string _nickname;
	std::string _passwordhash;
	mongo::BSONObj _gamearchive;
	int _adminlevel;
	bool _deleted;
	bool _banned;
	bool _signedin;
	mongo::OID _team;
	time_t _lastkill;
	int _killcounter;
	RolePtr _role;
	int _autosavetimer;
	boost::unordered_map<std::string, boost::shared_ptr<void>> _pvars;

public:
	Profile(int playerid, const mongo::OID& uniqueid);
	Profile(int playerid, const std::string& logname);
	~Profile();
	bool IsProfileDeleted();
	bool IsBannedForGame();
	void ApplyDataToPlayer();
	void Create(const std::string& logname, const std::string& password);
	void Sync();
	bool AuthPassword(const std::string& input) const;
	void SetPassword(const std::string& newpassword);
	CoordinatePlane GetPlaneCoordinate() const;
	std::string GetNickname() const;
	void SetNickname(const std::string& nickname);
	int GetAdminLevel() const;
	void SetAdminLevel(int level);
	Coordinate3D GetCameraFrontVector() const;
	Coordinate3D GetCameraPos() const;
	Coordinate3D GetPos() const;
	Coordinate3D GetVelocity() const;
	void SetBanned(bool banned);
	bool IsSignedIn() const;
	void SetSignedIn(bool signedin);
	Coordinate5D GetDetailedPos() const;
	mongo::OID GetTeamId() const;
	/* Changes OID. Use SetTeam() to set In-Game ID. */
	void SetTeamId(const mongo::OID& team);
	/* Returns the amount of killing in a row. */
	int KillCounter();
	void inline GiveScore(int score) {
		SetScore(GetScore() + score);
	}
	void PlaySound(int soundid);
	void SetRole(RolePtr& role);
	Profile::Role& GetRole();
	/* HP is limited to 100. */
	void inline GiveHealth(float health) {
		SetHealth(((100 - GetHealth()) > health) ? GetHealth() + health : 100);
	}

	template<typename Type>
	void SetVar(const std::string& key, const Type& value) {
		_pvars[key] = boost::shared_ptr<void>(new Type(value));
	}

	void SetVar(const std::string& key, const boost::shared_ptr<void>& value) {
		_pvars[key] = value;
	}

	template<typename Type>
	Type& GetVar(const std::string& key) {
		boost::unordered_map<std::string, boost::shared_ptr<void>>::iterator iter(_pvars.find(key));
		if(iter == _pvars.end()) throw std::runtime_error("Key doesn't exist");
		return *static_cast<Type*>(iter->second.get());
	}

	bool DelVar(const std::string& key) {
		return _pvars.erase(key) != 0;
	}

	bool HasVar(const std::string& key) {
		return _pvars.find(key) != _pvars.end();
	}
};

#endif
