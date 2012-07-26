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

#include "application_gamemode_sysmsgqueue.h"

SystemMessageQueue::SystemMessageQueue() : _currenttd(385.0f, 390.0f, " "), _old1sttd(385.0f, 375.0f, " "), _old2ndtd(385.0f, 359.0f, " ") {
	_currenttd.SetAlignment(2);
	_currenttd.SetFont(1);
	_currenttd.SetLetterSize(0.4f, 1.5f);
	_currenttd.SetProportional(true);
	_currenttd.SetSelectable(false);
	_old1sttd.SetAlignment(2);
	_old1sttd.SetFont(1);
	_old1sttd.SetLetterSize(0.3f, 1.2f);
	_old1sttd.SetProportional(true);
	_old1sttd.SetSelectable(false);
	_old2ndtd.SetAlignment(2);
	_old2ndtd.SetFont(1);
	_old2ndtd.SetLetterSize(0.3f, 1.2f);
	_old2ndtd.SetProportional(true);
	_old2ndtd.SetSelectable(false);
}

SystemMessageQueue& SystemMessageQueue::GetInstance() {
	static SystemMessageQueue inst;
	return inst;
}

void SystemMessageQueue::PushMessage(const std::string& msg) {
	_old2ndtd.SetString(_old1ststr);
	_old1sttd.SetString(_currentstr);
	_currenttd.SetString(msg);
	_old1ststr.assign(_currentstr);
	_currentstr.assign(msg);
	_currenttd.ShowForAll();
	_old1sttd.ShowForAll();
	_old2ndtd.ShowForAll();
}
