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

#ifndef FRESHCITY_COMMON_GBKENCODER
#define FRESHCITY_COMMON_GBKENCODER

#include "export.h"
#include <string>

FCEXPORT std::string GBKToUTF8(const std::string& strGBK);
FCEXPORT std::string UTF8ToGBK(const std::string& strUTF8);

#endif
