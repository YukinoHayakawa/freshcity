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

#ifndef FRESHCITY_BASIC_ALGORITHM_IDENTIFIER
#define FRESHCITY_BASIC_ALGORITHM_IDENTIFIER

#include <stack>

class IdentifierGenerator {
protected:
	int _max;
	int _count;
	std::stack<int> _returned;

public:
	IdentifierGenerator(int max = std::numeric_limits<int>::max())
		: _max(max), _count(0) {}

	int Get() {
		int id;
		if(!_returned.empty()) {
			id = _returned.top();
			_returned.pop();
		} else
			id = (_count == _max) ? 0 : ++_count;
		return id;
	}

	void Return(int id) {
		_returned.push(id);
	}
};

#endif
