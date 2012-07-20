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

#ifndef FRESHCITY_APPLICATION_GAMEMODE_MANAGER_BASE
#define FRESHCITY_APPLICATION_GAMEMODE_MANAGER_BASE

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/noncopyable.hpp>

template<class mgr, typename key, typename value>
class ItemManager : private boost::noncopyable {
public:
	typedef boost::shared_ptr<value> MemberPtr;

protected:
	typedef boost::unordered_map<key, MemberPtr> MemberMap;
	MemberMap _members;

public:
	bool virtual Add(const key& memberid, const MemberPtr& membercontent) {
		if(IsExist(memberid)) return false;
		_members.insert(std::make_pair(memberid, membercontent));
		return true;
	}

	bool IsExist(const key& memberid) const {
		return _members.find(memberid) != _members.end();
	}

	bool virtual Remove(const key& memberid) {
		MemberMap::iterator iter(_members.find(memberid));
		if(iter == _members.end()) return false;
		_members.erase(iter);
		return true;
	}

	value& Get(const key& memberid) {
		try {
			return *_members.at(memberid).get();
		} catch(...) {
			throw std::runtime_error("尝试获取不存在的对象");
		}
	}

	value& operator[](const key& memberid) {
		return Get(memberid);
	}

	static mgr& GetInstance() {
		static mgr inst;
		return inst;
	}
};

#endif
