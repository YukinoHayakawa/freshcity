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

#ifndef FRESHCITY_BASIC_ALGORITHM_MATH
#define FRESHCITY_BASIC_ALGORITHM_MATH

// Random
#include <boost/random.hpp>

__int64 inline Random(__int64 min, __int64 max) {
	boost::variate_generator<boost::mt19937&, boost::uniform_int<__int64>>
		gen(boost::mt19937((unsigned)clock()), boost::uniform_int<__int64>(min, max));
	return gen();
}

int inline RandomRGBAColor() { return (int)Random((__int64)0x00000000, (__int64)0xFFFFFFFF); }

int inline Random(int min, int max) {
	srand((unsigned)clock());
	return (rand() % (max - min)) + min;
}

int inline RandomWeather() { return Random(0, 22); }

// IdentifierAllocator
#include <stack>

class IdentifierAllocator {
protected:
	int _max;
	int _count;
	std::stack<int> _returned;

public:
	IdentifierAllocator(int max = std::numeric_limits<int>::max())
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
