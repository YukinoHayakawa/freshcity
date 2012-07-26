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

#ifndef FRESHCITY_APPLICATION_GAMEMODE_SYSMSGQUEUE
#define FRESHCITY_APPLICATION_GAMEMODE_SYSMSGQUEUE

#include <sampgdk/a_samp.h>
#include <boost/noncopyable.hpp>
#include <string>

class SystemMessageQueue : public boost::noncopyable {
private:
	std::string _currentstr, _old1ststr;
	TextDraw _currenttd, _old1sttd, _old2ndtd;
	SystemMessageQueue();

public:
	static SystemMessageQueue& GetInstance();
	void PushMessage(const std::string& msg);
};

#endif
