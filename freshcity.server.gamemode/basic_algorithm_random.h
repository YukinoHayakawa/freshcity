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

#ifndef FRESHCITY_BASIC_ALGORITHM_RANDOM
#define FRESHCITY_BASIC_ALGORITHM_RANDOM

#include <boost/random.hpp>

template<typename numbertype>
numbertype Random(numbertype min, numbertype max) {
	boost::variate_generator<boost::mt19937&, boost::uniform_real<numbertype>>
		gen(boost::mt19937((unsigned)clock()), boost::uniform_real<numbertype>(min, max));
	return gen();
}

int inline RandomRGBAColor() { return (int)Random<__int64>(0x00000000, 0xFFFFFFFF); }

#endif
