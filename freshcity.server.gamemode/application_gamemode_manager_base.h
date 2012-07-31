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
	typedef boost::unordered_map<key, MemberPtr> MemberMap;

protected:
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
		return _members.erase(memberid) != 0;
	}

	virtual value& Get(const key& memberid) {
		try {
			return *_members.at(memberid).get();
		} catch(...) {
			throw std::runtime_error("Object doesn't exist");
		}
	}

	virtual value& operator[](const key& memberid) {
		return Get(memberid);
	}

	static mgr& GetInstance() {
		static mgr inst;
		return inst;
	}

	typename MemberMap::iterator GetIterator() {
		return MemberMap::iterator(_members.begin());
	}
};

#define MANAGER_FOREACH(manager) \
	for(manager::MemberMap::iterator iter = manager::GetInstance().GetIterator(), end; \
	iter != end; iter++)

#define FETCH_ALL_FROM_DATABASE(col) std::auto_ptr<mongo::DBClientCursor> _cursor = \
	GetDB().query(CONFIG_STRING(col), mongo::BSONObj());\
	while(_cursor->more())

#endif
