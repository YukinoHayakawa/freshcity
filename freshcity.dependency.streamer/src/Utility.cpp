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
#include "Utility.h"

using namespace Utility;

cell AMX_NATIVE_CALL Utility::hookedNative(AMX *amx, cell *params)
{
	return 1;
}

boost::unordered_map<int, Element::SharedArea>::iterator Utility::destroyArea(boost::unordered_map<int, Element::SharedArea>::iterator a)
{
	Element::Area::identifier.remove(a->first, core->getData()->areas.size());
	for (boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
	{
		p->second.disabledAreas.erase(a->first);
		p->second.internalAreas.erase(a->first);
		p->second.visibleCell->areas.erase(a->first);
	}
	core->getGrid()->removeArea(a->second);
	return core->getData()->areas.erase(a);
}

boost::unordered_map<int, Element::SharedCheckpoint>::iterator Utility::destroyCheckpoint(boost::unordered_map<int, Element::SharedCheckpoint>::iterator c)
{
	Element::Checkpoint::identifier.remove(c->first, core->getData()->checkpoints.size());
	for (boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
	{
		if (p->second.visibleCheckpoint == c->first)
		{
			DisablePlayerCheckpoint(p->first);
			p->second.activeCheckpoint = 0;
			p->second.visibleCheckpoint = 0;
		}
		p->second.disabledCheckpoints.erase(c->first);
		p->second.visibleCell->checkpoints.erase(c->first);
	}
	core->getGrid()->removeCheckpoint(c->second);
	return core->getData()->checkpoints.erase(c);
}

boost::unordered_map<int, Element::SharedMapIcon>::iterator Utility::destroyMapIcon(boost::unordered_map<int, Element::SharedMapIcon>::iterator m)
{
	Element::MapIcon::identifier.remove(m->first, core->getData()->mapIcons.size());
	for (boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
	{
		boost::unordered_map<int, int>::iterator i = p->second.internalMapIcons.find(m->first);
		if (i != p->second.internalMapIcons.end())
		{
			RemovePlayerMapIcon(p->first, i->second);
			p->second.mapIconIdentifier.remove(i->second, p->second.internalMapIcons.size());
			p->second.internalMapIcons.quick_erase(i);
		}
		p->second.visibleCell->mapIcons.erase(m->first);
	}
	core->getGrid()->removeMapIcon(m->second);
	return core->getData()->mapIcons.erase(m);
}

boost::unordered_map<int, Element::SharedObject>::iterator Utility::destroyObject(boost::unordered_map<int, Element::SharedObject>::iterator o)
{
	Element::StreamerObject::identifier.remove(o->first, core->getData()->objects.size());
	for (boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
	{
		boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
		if (i != p->second.internalObjects.end())
		{
			DestroyPlayerObject(p->first, i->second);
			p->second.internalObjects.quick_erase(i);
		}
		p->second.visibleCell->objects.erase(o->first);
	}
	core->getGrid()->removeObject(o->second);
	return core->getData()->objects.erase(o);
}

boost::unordered_map<int, Element::SharedPickup>::iterator Utility::destroyPickup(boost::unordered_map<int, Element::SharedPickup>::iterator p)
{
	Element::Pickup::identifier.remove(p->first, core->getData()->pickups.size());
	boost::unordered_map<int, int>::iterator i = core->getStreamer()->internalPickups.find(p->first);
	if (i != core->getStreamer()->internalPickups.end())
	{
		DestroyPickup(i->second);
		core->getStreamer()->internalPickups.quick_erase(i);
	}
	core->getGrid()->removePickup(p->second);
	return core->getData()->pickups.erase(p);
}

boost::unordered_map<int, Element::SharedRaceCheckpoint>::iterator Utility::destroyRaceCheckpoint(boost::unordered_map<int, Element::SharedRaceCheckpoint>::iterator r)
{
	Element::RaceCheckpoint::identifier.remove(r->first, core->getData()->raceCheckpoints.size());
	for (boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
	{
		if (p->second.visibleRaceCheckpoint == r->first)
		{
			DisablePlayerRaceCheckpoint(p->first);
			p->second.activeRaceCheckpoint = 0;
			p->second.visibleRaceCheckpoint = 0;
		}
		p->second.disabledRaceCheckpoints.erase(r->first);
		p->second.visibleCell->raceCheckpoints.erase(r->first);
	}
	core->getGrid()->removeRaceCheckpoint(r->second);
	return core->getData()->raceCheckpoints.erase(r);
}

boost::unordered_map<int, Element::SharedTextLabel>::iterator Utility::destroyTextLabel(boost::unordered_map<int, Element::SharedTextLabel>::iterator t)
{
	Element::TextLabel::identifier.remove(t->first, core->getData()->textLabels.size());
	for (boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
	{
		boost::unordered_map<int, int>::iterator i = p->second.internalTextLabels.find(t->first);
		if (i != p->second.internalTextLabels.end())
		{
			DeletePlayer3DTextLabel(p->first, i->second);
			p->second.internalTextLabels.quick_erase(i);
		}
		p->second.visibleCell->textLabels.erase(t->first);
	}
	core->getGrid()->removeTextLabel(t->second);
	return core->getData()->textLabels.erase(t);
}

bool Utility::isPointInArea(const Eigen::Vector3f &point, const Element::SharedArea &area)
{
	switch (area->type)
	{
		case STREAMER_AREA_TYPE_CIRCLE:
		{
			if (area->attach)
			{
				if (boost::geometry::comparable_distance(Eigen::Vector2f(point[0], point[1]), Eigen::Vector2f(area->attach->position[0], area->attach->position[1])) <= area->size)
				{
					return true;
				}
			}
			else
			{
				if (boost::geometry::comparable_distance(Eigen::Vector2f(point[0], point[1]), boost::get<Eigen::Vector2f>(area->position)) <= area->size)
				{
					return true;
				}
			}
		}
		break;
		case STREAMER_AREA_TYPE_RECTANGLE:
		{
			return boost::geometry::within(Eigen::Vector2f(point[0], point[1]), boost::get<Element::Box2D>(area->position));
		}
		break;
		case STREAMER_AREA_TYPE_SPHERE:
		{
			if (area->attach)
			{
				if (boost::geometry::comparable_distance(point, area->attach->position) <= area->size)
				{
					return true;
				}
			}
			else
			{
				if (boost::geometry::comparable_distance(point, boost::get<Eigen::Vector3f>(area->position)) <= area->size)
				{
					return true;
				}
			}
		}
		break;
		case STREAMER_AREA_TYPE_CUBE:
		{
			return boost::geometry::within(point, boost::get<Element::Box3D>(area->position));
		}
		break;
		case STREAMER_AREA_TYPE_POLYGON:
		{
			if (point[2] >= boost::get<Element::Polygon2D>(area->position).get<1>()[0] && point[2] <= boost::get<Element::Polygon2D>(area->position).get<1>()[1])
			{
				return boost::geometry::within(Eigen::Vector2f(point[0], point[1]), boost::get<Element::Polygon2D>(area->position).get<0>());
			}
		}
		break;
	}
	return false;
}

void Utility::convertArrayToPolygon(AMX *amx, cell input, cell size, Element::Polygon2D &polygon)
{
	cell *array = NULL;
	std::vector<Eigen::Vector2f> points;
	amx_GetAddr(amx, input, &array);
	for (std::size_t i = 0; i < static_cast<std::size_t>(size); i += 2)
	{
		points.push_back(Eigen::Vector2f(amx_ctof(array[i]), amx_ctof(array[i + 1])));
	}
	boost::geometry::assign_points(polygon.get<0>(), points);
	boost::geometry::correct(polygon.get<0>());
}

std::string Utility::convertNativeStringToString(AMX *amx, cell input)
{
	char *string = NULL;
	amx_StrParam(amx, input, string);
	return string ? string : "";
}

void Utility::convertStringToNativeString(AMX *amx, cell output, cell size, std::string string)
{
	cell *address = NULL;
	amx_GetAddr(amx, output, &address);
	amx_SetString(address, string.c_str(), 0, 0, static_cast<size_t>(size));
}

void Utility::storeFloatInNative(AMX *amx, cell output, float number)
{
	cell *address;
	amx_GetAddr(amx, output, &address);
	*address = amx_ftoc(number);
}

void Utility::storeIntegerInNative(AMX *amx, cell output, int number)
{
	cell *address;
	amx_GetAddr(amx, output, &address);
	*address = number;
}
