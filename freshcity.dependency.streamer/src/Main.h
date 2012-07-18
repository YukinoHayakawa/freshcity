/*
    SA-MP Streamer Plugin v2.6.1
    Copyright ?2012 Incognito

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MAIN_H
#define MAIN_H

#define INCLUDE_FILE_VERSION (0x26104)
#define PLUGIN_VERSION "2.6.1"

#define STREAMER_TYPE_OBJECT (0)
#define STREAMER_TYPE_PICKUP (1)
#define STREAMER_TYPE_CP (2)
#define STREAMER_TYPE_RACE_CP (3)
#define STREAMER_TYPE_MAP_ICON (4)
#define STREAMER_TYPE_3D_TEXT_LABEL (5)
#define STREAMER_TYPE_AREA (6)

#define STREAMER_AREA_TYPE_CIRCLE (0)
#define STREAMER_AREA_TYPE_RECTANGLE (1)
#define STREAMER_AREA_TYPE_SPHERE (2)
#define STREAMER_AREA_TYPE_CUBE (3)
#define STREAMER_AREA_TYPE_POLYGON (4)

#define STREAMER_OBJECT_TYPE_GLOBAL (0)
#define STREAMER_OBJECT_TYPE_PLAYER (1)
#define STREAMER_OBJECT_TYPE_DYNAMIC (2)

#define BOOST_CHRONO_HEADER_ONLY

#include <bitset>
#include <cmath>
#include <limits>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include "../include/boost/chrono.hpp"
#include "../include/boost/geometry.hpp"
#include "../include/boost/geometry/geometries/geometries.hpp"
#include "../include/boost/intrusive_ptr.hpp"
#include "../include/boost/scoped_ptr.hpp"
#include "../include/boost/tuple/tuple.hpp"
#include "../include/boost/unordered_map.hpp"
#include "../include/boost/unordered_set.hpp"
#include "../include/boost/variant.hpp"

#include "../include/Eigen/Core"

#include "../include/sampgdk/a_objects.h"
#include "../include/sampgdk/a_players.h"
#include "../include/sampgdk/a_samp.h"
#include "../include/sampgdk/a_vehicles.h"
#include "../include/sampgdk/core.h"
#include "../include/sampgdk/plugin.h"

#include "Core.h"

#endif
