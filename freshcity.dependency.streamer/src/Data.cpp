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

#include "Main.h"

Data::Data()
{
	maxAreas = std::numeric_limits<int>::max();
	maxCheckpoints = std::numeric_limits<int>::max();
	maxMapIcons = std::numeric_limits<int>::max();
	maxObjects = std::numeric_limits<int>::max();
	maxPickups = std::numeric_limits<int>::max();
	maxRaceCheckpoints = std::numeric_limits<int>::max();
	maxTextLabels = std::numeric_limits<int>::max();
}

std::size_t Data::getMaxItems(int type)
{
	switch (type)
	{
		case STREAMER_TYPE_OBJECT:
		{
			return maxObjects;
		}
		break;
		case STREAMER_TYPE_PICKUP:
		{
			return maxPickups;
		}
		break;
		case STREAMER_TYPE_CP:
		{
			return maxCheckpoints;
		}
		break;
		case STREAMER_TYPE_RACE_CP:
		{
			return maxRaceCheckpoints;
		}
		break;
		case STREAMER_TYPE_MAP_ICON:
		{
			return maxMapIcons;
		}
		break;
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
			return maxTextLabels;
		}
		break;
		case STREAMER_TYPE_AREA:
		{
			return maxAreas;
		}
		break;
	}
	return 0;
}

bool Data::setMaxItems(int type, std::size_t value)
{
	switch (type)
	{
		case STREAMER_TYPE_OBJECT:
		{
			maxObjects = value;
			return true;
		}
		break;
		case STREAMER_TYPE_PICKUP:
		{
			maxPickups = value;
			return true;
		}
		break;
		case STREAMER_TYPE_CP:
		{
			maxCheckpoints = value;
			return true;
		}
		break;
		case STREAMER_TYPE_RACE_CP:
		{
			maxRaceCheckpoints = value;
			return true;
		}
		break;
		case STREAMER_TYPE_MAP_ICON:
		{
			maxMapIcons = value;
			return true;
		}
		break;
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
			maxTextLabels = value;
			return true;
		}
		break;
		case STREAMER_TYPE_AREA:
		{
			maxAreas = value;
			return true;
		}
		break;
	}
	return false;
}
