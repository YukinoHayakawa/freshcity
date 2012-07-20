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
#define STREAMER_EXPORT_IMPL
#include "Natives.h"
#include "Utility.h"

STREAMER_EXPORT bool Streamer_SetTickRate(int rate)
{
	return core->getStreamer()->setTickRate(rate);
}

STREAMER_EXPORT bool Streamer_SetMaxItems(int type, int items)
{
	return core->getData()->setMaxItems(type, items);

}

STREAMER_EXPORT bool Streamer_SetVisibleItems(int type, int items)
{
	return core->getStreamer()->setVisibleItems(type, items);
}

STREAMER_EXPORT void Streamer_SetCellDistance(float streamdistance)
{
	core->getGrid()->cellDistance = streamdistance * streamdistance;
	core->getGrid()->rebuildGrid();
}

STREAMER_EXPORT void Streamer_SetCellSize(float size)
{
	core->getGrid()->cellSize = size;
	core->getGrid()->rebuildGrid();
}

STREAMER_EXPORT void Streamer_ProcessActiveItems()
{
	core->getStreamer()->processActiveItems();
}

STREAMER_EXPORT bool Streamer_ToggleIdleUpdate(int playerid, int toggle)
{
	boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		p->second.idleUpdate = toggle != 0;
		return true;
	}
	return false;
}

STREAMER_EXPORT bool Streamer_Update(int playerid)
{
	boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		p->second.interiorID = GetPlayerInterior(p->first);
		p->second.worldID = GetPlayerVirtualWorld(p->first);
		GetPlayerPos(p->first, &p->second.position[0], &p->second.position[1], &p->second.position[2]);
		core->getStreamer()->startManualUpdate(p->second);
		return true;
	}
	return false;
}

STREAMER_EXPORT bool Streamer_UpdateEx(int playerid, float x, float y, float z, int worldid, int interiorid)
{
	boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		p->second.position = Eigen::Vector3f(x, y, z);
		if (worldid >= 0)
		{
			p->second.worldID = worldid;
		}
		else
		{
			p->second.worldID = GetPlayerVirtualWorld(p->first);
		}
		if (interiorid >= 0)
		{
			p->second.interiorID = interiorid;
		}
		else
		{
			p->second.interiorID = GetPlayerInterior(p->first);
		}
		core->getStreamer()->startManualUpdate(p->second);
		return true;
	}
	return false;
}

STREAMER_EXPORT int CreateDynamicObject(int modelid, float x, float y, float z, float rx, float ry, float rz, int worldid, int interiorid, int playerid, float streamdistance)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_OBJECT) == core->getData()->objects.size())
	{
		return false;
	}
	int objectID = Element::StreamerObject::identifier.get();
	Element::SharedObject object(new Element::StreamerObject);
	object->drawDistance = 0.0f;
	object->extraID = 0;
	object->objectID = objectID;
	object->modelID = modelid;
	object->position = Eigen::Vector3f(x, y, z);
	object->rotation = Eigen::Vector3f(rx, ry, rz);
	Utility::addToContainer(object->worlds, worldid);
	Utility::addToContainer(object->interiors, interiorid);
	Utility::addToContainer(object->players, playerid);
	object->streamDistance = streamdistance * streamdistance;
	core->getGrid()->addObject(object);
	core->getData()->objects.insert(std::make_pair(objectID, object));
	return objectID;
}

STREAMER_EXPORT bool DestroyDynamicObject(int objectid)
{
	boost::unordered_map<int, Element::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		Utility::destroyObject(o);
		return true;
	}
	return false;
}

STREAMER_EXPORT bool IsValidDynamicObject(int objectid)
{
	boost::unordered_map<int, Element::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		return true;
	}
	return false;
}

STREAMER_EXPORT bool SetDynamicObjectPos(int objectid, float x, float y, float z)
{
	boost::unordered_map<int, Element::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		Eigen::Vector3f position = o->second->position;
		o->second->position = Eigen::Vector3f(x, y, z);
		for (boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
			if (i != p->second.internalObjects.end())
			{
				SetPlayerObjectPos(p->first, i->second, o->second->position[0], o->second->position[1], o->second->position[2]);
			}
		}
		if (position[0] != o->second->position[0] || position[1] != o->second->position[1])
		{
			if (o->second->cell)
			{
				core->getGrid()->removeObject(o->second, true);
			}
		}
		if (o->second->move)
		{
			o->second->move->duration = static_cast<int>((boost::geometry::distance(o->second->move->position.get<0>(), o->second->position) / o->second->move->speed) * 1000.0f);
			o->second->move->position.get<1>() = o->second->position;
			o->second->move->position.get<2>() = (o->second->move->position.get<0>() - o->second->position) / static_cast<float>(o->second->move->duration);
			if (o->second->move->rotation.get<0>().maxCoeff() > -1000.0f)
			{
				o->second->move->rotation.get<1>() = o->second->rotation;
				o->second->move->rotation.get<2>() = (o->second->move->rotation.get<0>() - o->second->rotation) / static_cast<float>(o->second->move->duration);
			}
			o->second->move->time = boost::chrono::steady_clock::now();
		}
		return true;
	}
	return false;
}

STREAMER_EXPORT bool GetDynamicObjectPos(int objectid, float& x, float& y, float& z)
{
	boost::unordered_map<int, Element::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		x = o->second->position[0];
		y = o->second->position[1];
		z = o->second->position[2];
		return true;
	}
	return false;
}

STREAMER_EXPORT bool SetDynamicObjectRot(int objectid, float rx, float ry, float rz)
{
	boost::unordered_map<int, Element::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		if (o->second->move)
		{
			return false;
		}
		o->second->rotation = Eigen::Vector3f(rx, ry, rz);
		for (boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
			if (i != p->second.internalObjects.end())
			{
				SetPlayerObjectRot(p->first, i->second, o->second->rotation[0], o->second->rotation[1], o->second->rotation[2]);
			}
		}
		return true;
	}
	return false;
}

STREAMER_EXPORT bool GetDynamicObjectRot(int objectid, float& rx, float& ry, float& rz)
{
	boost::unordered_map<int, Element::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		rx = o->second->rotation[0];
		ry = o->second->rotation[1];
		rz = o->second->rotation[2];
		return true;
	}
	return false;
}

STREAMER_EXPORT int MoveDynamicObject(int objectid, float x, float y, float z, float speed, float rx, float ry, float rz)
{
	if (!speed)
	{
		return 0;
	}
	boost::unordered_map<int, Element::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		if (o->second->attach)
		{
			throw std::runtime_error("MoveDynamicObject: Object is currently attached and cannot be moved");
		}
		Eigen::Vector3f position(x, y, z);
		Eigen::Vector3f rotation(rx, ry, rz);
		o->second->move = boost::intrusive_ptr<Element::StreamerObject::Move>(new Element::StreamerObject::Move);
		o->second->move->duration = static_cast<int>((boost::geometry::distance(position, o->second->position) / speed) * 1000.0f);
		o->second->move->position.get<0>() = position;
		o->second->move->position.get<1>() = o->second->position;
		o->second->move->position.get<2>() = (position - o->second->position) / static_cast<float>(o->second->move->duration);
		o->second->move->rotation.get<0>() = rotation;
		if (o->second->move->rotation.get<0>().maxCoeff() > -1000.0f)
		{
			o->second->move->rotation.get<1>() = o->second->rotation;
			o->second->move->rotation.get<2>() = (rotation - o->second->rotation) / static_cast<float>(o->second->move->duration);
		}
		o->second->move->speed = speed;
		o->second->move->time = boost::chrono::steady_clock::now();
		for (boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
			if (i != p->second.internalObjects.end())
			{
				StopPlayerObject(p->first, i->second);
				MovePlayerObject(p->first, i->second, o->second->move->position.get<0>()[0], o->second->move->position.get<0>()[1], o->second->move->position.get<0>()[2], o->second->move->speed, o->second->move->rotation.get<0>()[0], o->second->move->rotation.get<0>()[1], o->second->move->rotation.get<0>()[2]);
			}
		}
		core->getStreamer()->movingObjects.insert(o->second);
		return o->second->move->duration;
	}
	return 0;
}

STREAMER_EXPORT bool StopDynamicObject(int objectid)
{
	boost::unordered_map<int, Element::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		if (o->second->move)
		{
			for (boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
			{
				boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
				if (i != p->second.internalObjects.end())
				{
					StopPlayerObject(p->first, i->second);
				}
			}
			o->second->move.reset();
			core->getStreamer()->movingObjects.erase(o->second);
			return true;
		}
	}
	return false;
}

STREAMER_EXPORT bool IsDynamicObjectMoving(int objectid)
{
	boost::unordered_map<int, Element::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		if (o->second->move)
		{
			return true;
		}
	}
	return false;
}

STREAMER_EXPORT bool AttachCameraToDynamicObject(int playerid, int objectid)
{
	boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(objectid);
		if (i != p->second.internalObjects.end())
		{
			AttachCameraToPlayerObject(p->first, i->second);
			return true;
		}
	}
	return false;
}

STREAMER_EXPORT bool AttachDynamicObjectToVehicle(int objectid, int vehicleid, float offsetx, float offsety, float offsetz, float rx, float ry, float rz)
{
	boost::unordered_map<int, Element::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		if (o->second->move)
		{
			throw std::runtime_error("AttachDynamicObjectToVehicle: Object is currently moving and cannot be attached");
		}
		o->second->attach = boost::intrusive_ptr<Element::StreamerObject::Attach>(new Element::StreamerObject::Attach);
		o->second->attach->vehicle = vehicleid;
		o->second->attach->offset = Eigen::Vector3f(offsetx, offsety, offsetz);
		o->second->attach->rotation = Eigen::Vector3f(rx, ry, rz);
		for (boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
			if (i != p->second.internalObjects.end())
			{
				AttachPlayerObjectToVehicle(p->first, i->second, o->second->attach->vehicle, o->second->attach->offset[0], o->second->attach->offset[1], o->second->attach->offset[2], o->second->attach->rotation[0], o->second->attach->rotation[1], o->second->attach->rotation[2]);
			}
		}
		if (vehicleid != INVALID_GENERIC_ID)
		{
			core->getStreamer()->attachedObjects.insert(o->second);
		}
		else
		{
			o->second->attach.reset();
			core->getStreamer()->attachedObjects.erase(o->second);
			core->getGrid()->removeObject(o->second, true);
		}
		return true;
	}
	return false;
}

STREAMER_EXPORT bool EditDynamicObject(int playerid, int objectid)
{
	boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(objectid);
		if (i != p->second.internalObjects.end())
		{
			EditPlayerObject(p->first, i->second);
			return true;
		}
	}
	return false;
}

STREAMER_EXPORT bool GetDynamicObjectMaterial(int objectid, int materialindex, int& modelid, std::string& txdname, std::string& texturename, int& materialcolor)
{
	boost::unordered_map<int, Element::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		boost::unordered_map<int, Element::StreamerObject::Material>::iterator m = o->second->materials.find(materialindex);
		if (m != o->second->materials.end())
		{
			if (m->second.main)
			{
				modelid = m->second.main->modelID;
				txdname = m->second.main->txdFileName;
				texturename = m->second.main->textureName;
				materialcolor = m->second.main->materialColor;
				return true;
			}
		}
	}
	return false;
}

STREAMER_EXPORT bool SetDynamicObjectMaterial(int objectid, int materialindex, int modelid, const std::string& txdname, const std::string& texturename, int materialcolor)
{
	boost::unordered_map<int, Element::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		o->second->materials[materialindex].main = boost::intrusive_ptr<Element::StreamerObject::Material::Main>(new Element::StreamerObject::Material::Main);
		o->second->materials[materialindex].main->modelID = modelid;
		o->second->materials[materialindex].main->txdFileName = txdname;
		o->second->materials[materialindex].main->textureName = texturename;
		o->second->materials[materialindex].main->materialColor = materialcolor;
		for (boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
			if (i != p->second.internalObjects.end())
			{
				SetPlayerObjectMaterial(p->first, i->second, materialindex, o->second->materials[materialindex].main->modelID, o->second->materials[materialindex].main->txdFileName.c_str(), o->second->materials[materialindex].main->textureName.c_str(), o->second->materials[materialindex].main->materialColor);
			}
		}
		o->second->materials[materialindex].text.reset();
		return true;
	}
	return false;
}

STREAMER_EXPORT bool GetDynamicObjectMaterialText(int objectid, int materialindex, std::string& text, int& materialsize, std::string& fontface, int& fontsize, bool& bold, int& fontcolor, int& backcolor, int& textalignment)
{
	boost::unordered_map<int, Element::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		boost::unordered_map<int, Element::StreamerObject::Material>::iterator m = o->second->materials.find(materialindex);
		if (m != o->second->materials.end())
		{
			if (m->second.text)
			{
				text = m->second.text->materialText;
				materialsize = m->second.text->materialSize;
				fontface = m->second.text->fontFace;
				fontsize = m->second.text->fontSize;
				bold = m->second.text->bold;
				fontcolor = m->second.text->fontColor;
				backcolor = m->second.text->backColor;
				textalignment = m->second.text->textAlignment;
				return true;
			}
		}
	}
	return false;
}

STREAMER_EXPORT bool SetDynamicObjectMaterialText(int objectid, int materialindex, const std::string& text, int materialsize, const std::string& fontface, int fontsize, bool bold, int fontcolor, int backcolor, int textalignment)
{
	boost::unordered_map<int, Element::SharedObject>::iterator o = core->getData()->objects.find(objectid);
	if (o != core->getData()->objects.end())
	{
		int index = materialindex;
		o->second->materials[index].text = boost::intrusive_ptr<Element::StreamerObject::Material::Text>(new Element::StreamerObject::Material::Text);
		o->second->materials[index].text->materialText = text;
		o->second->materials[index].text->materialSize = materialsize;
		o->second->materials[index].text->fontFace = fontface;
		o->second->materials[index].text->fontSize = fontsize;
		o->second->materials[index].text->bold = bold;
		o->second->materials[index].text->fontColor = fontcolor;
		o->second->materials[index].text->backColor = backcolor;
		o->second->materials[index].text->textAlignment = textalignment;
		for (boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalObjects.find(o->first);
			if (i != p->second.internalObjects.end())
			{
				SetPlayerObjectMaterialText(p->first, i->second, o->second->materials[index].text->materialText.c_str(), index, o->second->materials[index].text->materialSize, o->second->materials[index].text->fontFace.c_str(), o->second->materials[index].text->fontSize, o->second->materials[index].text->bold, o->second->materials[index].text->fontColor, o->second->materials[index].text->backColor, o->second->materials[index].text->textAlignment);
			}
		}
		o->second->materials[index].main.reset();
		return true;
	}
	return false;
}

STREAMER_EXPORT void DestroyAllDynamicObjects()
{
	Element::StreamerObject::identifier.reset();
	for (boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
	{
		for (boost::unordered_map<int, int>::iterator o = p->second.internalObjects.begin(); o != p->second.internalObjects.end(); ++o)
		{
			DestroyPlayerObject(p->first, o->second);
		}
		p->second.internalObjects.clear();
	}
	core->getGrid()->eraseAllItems(STREAMER_TYPE_OBJECT);
	core->getStreamer()->attachedObjects.clear();
	core->getStreamer()->movingObjects.clear();
	core->getData()->objects.clear();
}

STREAMER_EXPORT int CountDynamicObjects()
{
	return core->getData()->objects.size();
}

STREAMER_EXPORT int CreateDynamicPickup(int modelid, int type, float x, float y, float z, int worldid, int interiorid, int playerid, float streamdistance)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_PICKUP) == core->getData()->pickups.size())
	{
		return 0;
	}
	int pickupID = Element::Pickup::identifier.get();
	Element::SharedPickup pickup(new Element::Pickup);
	pickup->extraID = 0;
	pickup->pickupID = pickupID;
	pickup->worldID = 0;
	pickup->modelID = modelid;
	pickup->type = type;
	pickup->position = Eigen::Vector3f(x, y, z);
	Utility::addToContainer(pickup->worlds, worldid);
	Utility::addToContainer(pickup->interiors, interiorid);
	Utility::addToContainer(pickup->players, playerid);
	pickup->streamDistance = streamdistance * streamdistance;
	core->getGrid()->addPickup(pickup);
	core->getData()->pickups.insert(std::make_pair(pickupID, pickup));
	return pickupID;
}

STREAMER_EXPORT bool DestroyDynamicPickup(int pickupid)
{
	boost::unordered_map<int, Element::SharedPickup>::iterator p = core->getData()->pickups.find(pickupid);
	if (p != core->getData()->pickups.end())
	{
		Utility::destroyPickup(p);
		return true;
	}
	return false;
}

STREAMER_EXPORT bool IsValidDynamicPickup(int pickupid)
{
	boost::unordered_map<int, Element::SharedPickup>::iterator p = core->getData()->pickups.find(pickupid);
	if (p != core->getData()->pickups.end())
	{
		return true;
	}
	return false;
}

STREAMER_EXPORT void DestroyAllDynamicPickups()
{
	Element::Pickup::identifier.reset();
	for (boost::unordered_map<int, int>::iterator p = core->getStreamer()->internalPickups.begin(); p != core->getStreamer()->internalPickups.end(); ++p)
	{
		DestroyPickup(p->second);
	}
	core->getStreamer()->internalPickups.clear();
	core->getGrid()->eraseAllItems(STREAMER_TYPE_PICKUP);
	core->getData()->pickups.clear();
}

STREAMER_EXPORT int CountDynamicPickups()
{
	return core->getData()->pickups.size();
}

STREAMER_EXPORT int CreateDynamicCP(float x, float y, float z, float size, int worldid, int interiorid, int playerid, float streamdistance)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_CP) == core->getData()->checkpoints.size())
	{
		return 0;
	}
	int checkpointID = Element::Checkpoint::identifier.get();
	Element::SharedCheckpoint checkpoint(new Element::Checkpoint);
	checkpoint->checkpointID = checkpointID;
	checkpoint->extraID = 0;
	checkpoint->position = Eigen::Vector3f(x, y, z);
	checkpoint->size = size;
	Utility::addToContainer(checkpoint->worlds, worldid);
	Utility::addToContainer(checkpoint->interiors, interiorid);
	Utility::addToContainer(checkpoint->players, playerid);
	checkpoint->streamDistance = streamdistance * streamdistance;
	core->getGrid()->addCheckpoint(checkpoint);
	core->getData()->checkpoints.insert(std::make_pair(checkpointID, checkpoint));
	return checkpointID;
}

STREAMER_EXPORT bool DestroyDynamicCP(int checkpointid)
{
	boost::unordered_map<int, Element::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(checkpointid);
	if (c != core->getData()->checkpoints.end())
	{
		Utility::destroyCheckpoint(c);
		return true;
	}
	return false;
}

STREAMER_EXPORT bool IsValidDynamicCP(int checkpointid)
{
	boost::unordered_map<int, Element::SharedCheckpoint>::iterator c = core->getData()->checkpoints.find(checkpointid);
	if (c != core->getData()->checkpoints.end())
	{
		return true;
	}
	return false;
}

STREAMER_EXPORT bool TogglePlayerDynamicCP(int playerid, int checkpointid, bool toggle)
{
	boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		boost::unordered_set<int>::iterator d = p->second.disabledCheckpoints.find(checkpointid);
		if (toggle)
		{
			if (d != p->second.disabledCheckpoints.end())
			{
				p->second.disabledCheckpoints.quick_erase(d);
				return true;
			}
		}
		else
		{
			if (d == p->second.disabledCheckpoints.end())
			{
				if (p->second.visibleCheckpoint == checkpointid)
				{
					DisablePlayerCheckpoint(p->first);
					p->second.activeCheckpoint = 0;
					p->second.visibleCheckpoint = 0;
				}
				p->second.disabledCheckpoints.insert(checkpointid);
				return true;
			}
		}
	}
	return false;
}

STREAMER_EXPORT bool TogglePlayerAllDynamicCPs(int playerid, bool toggle)
{
	boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		p->second.disabledCheckpoints.clear();
		if (toggle)
		{
			if (p->second.visibleCheckpoint != 0)
			{
				DisablePlayerCheckpoint(p->first);
				p->second.activeCheckpoint = 0;
				p->second.visibleCheckpoint = 0;
			}
			for (boost::unordered_map<int, Element::SharedCheckpoint>::iterator c = core->getData()->checkpoints.begin(); c != core->getData()->checkpoints.end(); ++c)
			{
				p->second.disabledCheckpoints.insert(c->first);
			}
		}
		return true;
	}
	return false;
}

STREAMER_EXPORT bool IsPlayerInDynamicCP(int playerid, int checkpointid)
{
	boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		if (p->second.activeCheckpoint == checkpointid)
		{
			return true;
		}
	}
	return false;
}

STREAMER_EXPORT int GetPlayerVisibleDynamicCP(int playerid)
{
	boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		return p->second.visibleCheckpoint;
	}
	return false;
}

STREAMER_EXPORT void DestroyAllDynamicCPs()
{
	Element::Checkpoint::identifier.reset();
	for (boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
	{
		p->second.disabledCheckpoints.clear();
		if (p->second.visibleCheckpoint != 0)
		{
			DisablePlayerCheckpoint(p->first);
			p->second.activeCheckpoint = 0;
			p->second.visibleCheckpoint = 0;
		}
	}
	core->getGrid()->eraseAllItems(STREAMER_TYPE_CP);
	core->getData()->checkpoints.clear();
}

STREAMER_EXPORT int CountDynamicCPs()
{
	return core->getData()->checkpoints.size();
}

STREAMER_EXPORT int CreateDynamicRaceCP(int type, float x, float y, float z, float nextx, float nexty, float nextz, float size, int worldid, int interiorid, int playerid, float streamdistance)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_RACE_CP) == core->getData()->raceCheckpoints.size())
	{
		return 0;
	}
	int raceCheckpointID = Element::RaceCheckpoint::identifier.get();
	Element::SharedRaceCheckpoint raceCheckpoint(new Element::RaceCheckpoint);
	raceCheckpoint->extraID = 0;
	raceCheckpoint->raceCheckpointID = raceCheckpointID;
	raceCheckpoint->type = type;
	raceCheckpoint->position = Eigen::Vector3f(x, y, z);
	raceCheckpoint->next = Eigen::Vector3f(nextx, nexty, nextz);
	raceCheckpoint->size = size;
	Utility::addToContainer(raceCheckpoint->worlds, worldid);
	Utility::addToContainer(raceCheckpoint->interiors, interiorid);
	Utility::addToContainer(raceCheckpoint->players, playerid);
	raceCheckpoint->streamDistance = streamdistance * streamdistance;
	core->getGrid()->addRaceCheckpoint(raceCheckpoint);
	core->getData()->raceCheckpoints.insert(std::make_pair(raceCheckpointID, raceCheckpoint));
	return raceCheckpointID;
}

STREAMER_EXPORT bool DestroyDynamicRaceCP(int checkpointid)
{
	boost::unordered_map<int, Element::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(checkpointid);
	if (r != core->getData()->raceCheckpoints.end())
	{
		Utility::destroyRaceCheckpoint(r);
		return true;
	}
	return false;
}

STREAMER_EXPORT bool IsValidDynamicRaceCP(int checkpointid)
{
	boost::unordered_map<int, Element::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.find(checkpointid);
	if (r != core->getData()->raceCheckpoints.end())
	{
		return true;
	}
	return false;
}

STREAMER_EXPORT bool TogglePlayerDynamicRaceCP(int playerid, int checkpointid, bool toggle)
{
	boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		boost::unordered_set<int>::iterator d = p->second.disabledRaceCheckpoints.find(checkpointid);
		if (toggle)
		{
			if (d != p->second.disabledRaceCheckpoints.end())
			{
				p->second.disabledRaceCheckpoints.quick_erase(d);
				return true;
			}
		}
		else
		{
			if (d == p->second.disabledRaceCheckpoints.end())
			{
				if (p->second.visibleRaceCheckpoint == checkpointid)
				{
					DisablePlayerRaceCheckpoint(p->first);
					p->second.activeRaceCheckpoint = 0;
					p->second.visibleRaceCheckpoint = 0;
				}
				p->second.disabledRaceCheckpoints.insert(checkpointid);
				return true;
			}
		}
	}
	return false;
}

STREAMER_EXPORT bool TogglePlayerAllDynamicRaceCPs(int playerid, bool toggle)
{
	boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		p->second.disabledRaceCheckpoints.clear();
		if (!toggle)
		{
			if (p->second.visibleRaceCheckpoint != 0)
			{
				DisablePlayerRaceCheckpoint(p->first);
				p->second.activeRaceCheckpoint = 0;
				p->second.visibleRaceCheckpoint = 0;
			}
			for (boost::unordered_map<int, Element::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.begin(); r != core->getData()->raceCheckpoints.end(); ++r)
			{
				p->second.disabledRaceCheckpoints.insert(r->first);
			}
		}
		return true;
	}
	return false;
}

STREAMER_EXPORT bool IsPlayerInDynamicRaceCP(int playerid, int checkpointid)
{
	boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		if (p->second.activeRaceCheckpoint == checkpointid)
		{
			return true;
		}
	}
	return false;
}

STREAMER_EXPORT int GetPlayerVisibleDynamicRaceCP(int playerid)
{
	boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		return p->second.visibleRaceCheckpoint;
	}
	return 0;
}

STREAMER_EXPORT void DestroyAllDynamicRaceCPs()
{
	Element::RaceCheckpoint::identifier.reset();
	for (boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
	{
		p->second.disabledRaceCheckpoints.clear();
		if (p->second.visibleRaceCheckpoint != 0)
		{
			DisablePlayerRaceCheckpoint(p->first);
			p->second.activeRaceCheckpoint = 0;
			p->second.visibleRaceCheckpoint = 0;
		}
	}
	core->getGrid()->eraseAllItems(STREAMER_TYPE_RACE_CP);
	core->getData()->raceCheckpoints.clear();
}

STREAMER_EXPORT int CountDynamicRaceCPs()
{
	return core->getData()->raceCheckpoints.size();
}

STREAMER_EXPORT int CreateDynamicMapIcon(float x, float y, float z, int type, int color, int worldid, int interiorid, int playerid, float streamdistance)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_MAP_ICON) == core->getData()->mapIcons.size())
	{
		return 0;
	}
	int mapIconID = Element::MapIcon::identifier.get();
	Element::SharedMapIcon mapIcon(new Element::MapIcon);
	mapIcon->extraID = 0;
	mapIcon->mapIconID = mapIconID;
	mapIcon->style = 0;
	mapIcon->position = Eigen::Vector3f(x, y, z);
	mapIcon->type = type;
	mapIcon->color = color;
	Utility::addToContainer(mapIcon->worlds, worldid);
	Utility::addToContainer(mapIcon->interiors, interiorid);
	Utility::addToContainer(mapIcon->players, playerid);
	mapIcon->streamDistance = streamdistance * streamdistance;
	core->getGrid()->addMapIcon(mapIcon);
	core->getData()->mapIcons.insert(std::make_pair(mapIconID, mapIcon));
	return mapIconID;
}

STREAMER_EXPORT bool DestroyDynamicMapIcon(int iconid)
{
	boost::unordered_map<int, Element::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(iconid);
	if (m != core->getData()->mapIcons.end())
	{
		Utility::destroyMapIcon(m);
		return true;
	}
	return false;
}

STREAMER_EXPORT bool IsValidDynamicMapIcon(int iconid)
{
	boost::unordered_map<int, Element::SharedMapIcon>::iterator m = core->getData()->mapIcons.find(iconid);
	if (m != core->getData()->mapIcons.end())
	{
		return true;
	}
	return false;
}

STREAMER_EXPORT void DestroyAllDynamicMapIcons()
{
	Element::MapIcon::identifier.reset();
	for (boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
	{
		for (boost::unordered_map<int, int>::iterator m = p->second.internalMapIcons.begin(); m != p->second.internalMapIcons.end(); ++m)
		{
			RemovePlayerMapIcon(p->first, m->second);
		}
		p->second.mapIconIdentifier.reset();
		p->second.internalMapIcons.clear();
	}
	core->getGrid()->eraseAllItems(STREAMER_TYPE_MAP_ICON);
	core->getData()->mapIcons.clear();
}

STREAMER_EXPORT int CountDynamicMapIcons()
{
	return core->getData()->mapIcons.size();
}

STREAMER_EXPORT int CreateDynamic3DTextLabel(const std::string& text, int color, float x, float y, float z, float drawdistance, int attachedplayer, int attachedvehicle, int testlos, int worldid, int interiorid, int playerid, float streamdistance)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_3D_TEXT_LABEL) == core->getData()->textLabels.size())
	{
		return 0;
	}
	int textLabelID = Element::TextLabel::identifier.get();
	Element::SharedTextLabel textLabel(new Element::TextLabel);
	textLabel->extraID = 0;
	textLabel->textLabelID = textLabelID;
	textLabel->text = text;
	textLabel->color = color;
	textLabel->position = Eigen::Vector3f(x, y, z);
	textLabel->drawDistance = drawdistance;
	if (attachedplayer != INVALID_GENERIC_ID || attachedvehicle != INVALID_GENERIC_ID)
	{
		textLabel->attach = boost::intrusive_ptr<Element::TextLabel::Attach>(new Element::TextLabel::Attach);
		textLabel->attach->player = attachedplayer;
		textLabel->attach->vehicle = attachedvehicle;
		if (textLabel->position.cwiseAbs().maxCoeff() > 50.0f)
		{
			textLabel->position.setZero();
		}
		core->getStreamer()->attachedTextLabels.insert(textLabel);
	}
	textLabel->testLOS = testlos != 0;
	Utility::addToContainer(textLabel->worlds, worldid);
	Utility::addToContainer(textLabel->interiors, interiorid);
	Utility::addToContainer(textLabel->players, playerid);
	textLabel->streamDistance = streamdistance * streamdistance;
	core->getGrid()->addTextLabel(textLabel);
	core->getData()->textLabels.insert(std::make_pair(textLabelID, textLabel));
	return textLabelID;
}

STREAMER_EXPORT bool DestroyDynamic3DTextLabel(int id)
{
	boost::unordered_map<int, Element::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
	if (t != core->getData()->textLabels.end())
	{
		Utility::destroyTextLabel(t);
		return true;
	}
	return false;
}

STREAMER_EXPORT bool IsValidDynamic3DTextLabel(int id)
{
	boost::unordered_map<int, Element::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
	if (t != core->getData()->textLabels.end())
	{
		return true;
	}
	return false;
}

STREAMER_EXPORT bool GetDynamic3DTextLabelText(int id, std::string& text)
{
	boost::unordered_map<int, Element::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
	if (t != core->getData()->textLabels.end())
	{
		text = t->second->text;
		return true;
	}
	return false;
}

STREAMER_EXPORT bool UpdateDynamic3DTextLabelText(int id, int color, const std::string& text)
{
	boost::unordered_map<int, Element::SharedTextLabel>::iterator t = core->getData()->textLabels.find(id);
	if (t != core->getData()->textLabels.end())
	{
		t->second->color = color;
		t->second->text = text;
		for (boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
		{
			boost::unordered_map<int, int>::iterator i = p->second.internalTextLabels.find(t->first);
			if (i != p->second.internalTextLabels.end())
			{
				UpdatePlayer3DTextLabelText(p->first, i->second, t->second->color, t->second->text.c_str());
			}
		}
		return true;
	}
	return false;
}

STREAMER_EXPORT void DestroyAllDynamic3DTextLabels()
{
	Element::TextLabel::identifier.reset();
	for (boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
	{
		for (boost::unordered_map<int, int>::iterator t = p->second.internalTextLabels.begin(); t != p->second.internalTextLabels.end(); ++t)
		{
			DeletePlayer3DTextLabel(p->first, t->second);
		}
		p->second.internalTextLabels.clear();
	}
	core->getGrid()->eraseAllItems(STREAMER_TYPE_3D_TEXT_LABEL);
	core->getStreamer()->attachedTextLabels.clear();
	core->getData()->textLabels.clear();
}

STREAMER_EXPORT int CountDynamic3DTextLabels()
{
	return core->getData()->textLabels.size();
}

STREAMER_EXPORT int CreateDynamicCircle(float x, float y, float size, int worldid, int interiorid, int playerid)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
	{
		return 0;
	}
	int areaID = Element::Area::identifier.get();
	Element::SharedArea area(new Element::Area);
	area->areaID = areaID;
	area->extraID = 0;
	area->type = STREAMER_AREA_TYPE_CIRCLE;
	area->position = Eigen::Vector2f(x, y);
	area->size = size * size;
	Utility::addToContainer(area->worlds, worldid);
	Utility::addToContainer(area->interiors, interiorid);
	Utility::addToContainer(area->players, playerid);
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return areaID;
}

STREAMER_EXPORT int CreateDynamicRectangle(float minx, float miny, float maxx, float maxy, int worldid, int interiorid, int playerid)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
	{
		return 0;
	}
	int areaID = Element::Area::identifier.get();
	Element::SharedArea area(new Element::Area);
	area->areaID = areaID;
	area->extraID = 0;
	area->type = STREAMER_AREA_TYPE_RECTANGLE;
	area->position = Element::Box2D(Eigen::Vector2f(minx, miny), Eigen::Vector2f(maxx, maxy));
	boost::geometry::correct(boost::get<Element::Box2D>(area->position));
	area->size = boost::geometry::comparable_distance(boost::get<Element::Box2D>(area->position).min_corner(), boost::get<Element::Box2D>(area->position).max_corner());
	Utility::addToContainer(area->worlds, worldid);
	Utility::addToContainer(area->interiors, interiorid);
	Utility::addToContainer(area->players, playerid);
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return areaID;
}

STREAMER_EXPORT int CreateDynamicSphere(float x, float y, float z, float size, int worldid, int interiorid, int playerid)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
	{
		return 0;
	}
	int areaID = Element::Area::identifier.get();
	Element::SharedArea area(new Element::Area);
	area->areaID = areaID;
	area->extraID = 0;
	area->type = STREAMER_AREA_TYPE_SPHERE;
	area->position = Eigen::Vector3f(x, y, z);
	area->size = size * size;
	Utility::addToContainer(area->worlds, worldid);
	Utility::addToContainer(area->interiors, interiorid);
	Utility::addToContainer(area->players, playerid);
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return areaID;
}

STREAMER_EXPORT int CreateDynamicCube(float minx, float miny, float minz, float maxx, float maxy, float maxz, int worldid, int interiorid, int playerid)
{
	if (core->getData()->getMaxItems(STREAMER_TYPE_AREA) == core->getData()->areas.size())
	{
		return 0;
	}
	int areaID = Element::Area::identifier.get();
	Element::SharedArea area(new Element::Area);
	area->areaID = areaID;
	area->extraID = 0;
	area->type = STREAMER_AREA_TYPE_CUBE;
	area->position = Element::Box3D(Eigen::Vector3f(minx, miny, minz), Eigen::Vector3f(maxx, maxy, maxz));
	boost::geometry::correct(boost::get<Element::Box3D>(area->position));
	area->size = boost::geometry::comparable_distance(Eigen::Vector2f(boost::get<Element::Box3D>(area->position).min_corner()[0], boost::get<Element::Box3D>(area->position).min_corner()[1]), Eigen::Vector2f(boost::get<Element::Box3D>(area->position).max_corner()[0], boost::get<Element::Box3D>(area->position).max_corner()[1]));
	Utility::addToContainer(area->worlds, worldid);
	Utility::addToContainer(area->interiors, interiorid);
	Utility::addToContainer(area->players, playerid);
	core->getGrid()->addArea(area);
	core->getData()->areas.insert(std::make_pair(areaID, area));
	return areaID;
}

STREAMER_EXPORT bool DestroyDynamicArea(int areaid)
{
	boost::unordered_map<int, Element::SharedArea>::iterator a = core->getData()->areas.find(areaid);
	if (a != core->getData()->areas.end())
	{
		Utility::destroyArea(a);
		return true;
	}
	return false;
}

STREAMER_EXPORT bool IsValidDynamicArea(int areaid)
{
	boost::unordered_map<int, Element::SharedArea>::iterator a = core->getData()->areas.find(areaid);
	if (a != core->getData()->areas.end())
	{
		return true;
	}
	return false;
}

STREAMER_EXPORT bool TogglePlayerDynamicArea(int playerid, int areaid, bool toggle)
{
	boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		boost::unordered_set<int>::iterator d = p->second.disabledAreas.find(areaid);
		if (toggle)
		{
			if (d != p->second.disabledAreas.end())
			{
				p->second.disabledAreas.quick_erase(d);
				return true;
			}
		}
		else
		{
			if (d == p->second.disabledAreas.end())
			{
				p->second.disabledAreas.insert(areaid);
				p->second.internalAreas.erase(areaid);
				return true;
			}
		}
	}
	return false;
}

STREAMER_EXPORT bool TogglePlayerAllDynamicAreas(int playerid, bool toggle)
{
	boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		p->second.disabledAreas.clear();
		if (!toggle)
		{
			for (boost::unordered_map<int, Element::SharedArea>::iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a)
			{
				p->second.disabledAreas.insert(a->first);
			}
			p->second.internalAreas.clear();
		}
		return true;
	}
	return false;
}

STREAMER_EXPORT bool IsPlayerInDynamicArea(int playerid, int areaid)
{
	boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		boost::unordered_set<int>::iterator i = p->second.internalAreas.find(areaid);
		if (i != p->second.internalAreas.end())
		{
			return true;
		}
	}
	return false;
}

STREAMER_EXPORT bool IsPlayerInAnyDynamicArea(int playerid)
{
	boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.find(playerid);
	if (p != core->getData()->players.end())
	{
		if (!p->second.internalAreas.empty())
		{
			return true;
		}
	}
	return false;
}

STREAMER_EXPORT bool IsPointInDynamicArea(int areaid, float x, float y, float z)
{
	boost::unordered_map<int, Element::SharedArea>::iterator a = core->getData()->areas.find(areaid);
	if (a != core->getData()->areas.end())
	{
		return Utility::isPointInArea(Eigen::Vector3f(x, y, z), a->second);
	}
	return false;
}

STREAMER_EXPORT bool IsPointInAnyDynamicArea(float x, float y, float z)
{
	for (boost::unordered_map<int, Element::SharedArea>::const_iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a)
	{
		if (Utility::isPointInArea(Eigen::Vector3f(x, y, z), a->second))
		{
			return true;
		}
	}
	return false;
}

STREAMER_EXPORT bool AttachDynamicAreaToObject(int areaid, int objectid, int type, int playerid)
{
	boost::unordered_map<int, Element::SharedArea>::iterator a = core->getData()->areas.find(areaid);
	if (a != core->getData()->areas.end())
	{
		if (a->second->type != STREAMER_AREA_TYPE_CIRCLE && a->second->type != STREAMER_AREA_TYPE_SPHERE)
		{
			throw std::runtime_error("*** AttachDynamicAreaToObject: Only circles and spheres may be attached to objects");
		}
		if (objectid != INVALID_GENERIC_ID)
		{
			a->second->attach = boost::intrusive_ptr<Element::Area::Attach>(new Element::Area::Attach);
			a->second->attach->object = boost::make_tuple(objectid, type, playerid);
			a->second->attach->player = INVALID_GENERIC_ID;
			a->second->attach->vehicle = INVALID_GENERIC_ID;
			core->getStreamer()->attachedAreas.insert(a->second);
		}
		else
		{
			if (a->second->attach)
			{
				if (a->second->attach->object.get<0>() != INVALID_GENERIC_ID)
				{
					a->second->attach.reset();
					core->getStreamer()->attachedAreas.erase(a->second);
					core->getGrid()->removeArea(a->second, true);
				}
			}
		}
		return true;
	}
	return false;
}

STREAMER_EXPORT bool AttachDynamicAreaToPlayer(int areaid, int playerid)
{
	boost::unordered_map<int, Element::SharedArea>::iterator a = core->getData()->areas.find(areaid);
	if (a != core->getData()->areas.end())
	{
		if (a->second->type != STREAMER_AREA_TYPE_CIRCLE && a->second->type != STREAMER_AREA_TYPE_SPHERE)
		{
			throw std::runtime_error("*** AttachDynamicAreaToPlayer: Only circles and spheres may be attached to players");
		}
		if (playerid != INVALID_GENERIC_ID)
		{
			a->second->attach = boost::intrusive_ptr<Element::Area::Attach>(new Element::Area::Attach);
			a->second->attach->object.get<0>() = INVALID_GENERIC_ID;
			a->second->attach->player = playerid;
			a->second->attach->vehicle = INVALID_GENERIC_ID;
			core->getStreamer()->attachedAreas.insert(a->second);
		}
		else
		{
			if (a->second->attach)
			{
				if (a->second->attach->player != INVALID_GENERIC_ID)
				{
					a->second->attach.reset();
					core->getStreamer()->attachedAreas.erase(a->second);
					core->getGrid()->removeArea(a->second, true);
				}
			}
		}
		return true;
	}
	return false;
}

STREAMER_EXPORT bool AttachDynamicAreaToVehicle(int areaid, int vehicleid)
{
	boost::unordered_map<int, Element::SharedArea>::iterator a = core->getData()->areas.find(areaid);
	if (a != core->getData()->areas.end())
	{
		if (a->second->type != STREAMER_AREA_TYPE_CIRCLE && a->second->type != STREAMER_AREA_TYPE_SPHERE)
		{
			throw std::runtime_error("*** AttachDynamicAreaToVehicle: Only circles and spheres may be attached to vehicles");
			return false;
		}
		if (vehicleid != INVALID_GENERIC_ID)
		{
			a->second->attach = boost::intrusive_ptr<Element::Area::Attach>(new Element::Area::Attach);
			a->second->attach->object.get<0>() = INVALID_GENERIC_ID;
			a->second->attach->player = INVALID_GENERIC_ID;
			a->second->attach->vehicle = vehicleid;
			core->getStreamer()->attachedAreas.insert(a->second);
		}
		else
		{
			if (a->second->attach)
			{
				if (a->second->attach->vehicle != INVALID_GENERIC_ID)
				{
					a->second->attach.reset();
					core->getStreamer()->attachedAreas.erase(a->second);
					core->getGrid()->removeArea(a->second, true);
				}
			}
		}
		return true;
	}
	return false;
}

STREAMER_EXPORT void DestroyAllDynamicAreas()
{
	Element::Area::identifier.reset();
	for (boost::unordered_map<int, StreamerPlayer>::iterator p = core->getData()->players.begin(); p != core->getData()->players.end(); ++p)
	{
		p->second.disabledAreas.clear();
		p->second.internalAreas.clear();
	}
	core->getGrid()->eraseAllItems(STREAMER_TYPE_AREA);
	core->getStreamer()->attachedAreas.clear();
	core->getData()->areas.clear();
}

STREAMER_EXPORT int CountDynamicAreas()
{
	return core->getData()->areas.size();
}

STREAMER_EXPORT bool Streamer_OnPlayerConnect(int playerid)
{
	return core->getEvents()->OnPlayerConnect(playerid);
}

STREAMER_EXPORT bool Streamer_OnPlayerDisconnect(int playerid, int reason)
{
	return core->getEvents()->OnPlayerDisconnect(playerid, reason);
}

STREAMER_EXPORT int Streamer_OnPlayerEditObject(int playerid, bool playerobject, int objectid, int response, float fX, float fY, float fZ, float fRotX, float fRotY, float fRotZ)
{
	return core->getEvents()->OnPlayerEditObject(playerid, playerobject, objectid, response, fX, fY, fZ, fRotX, fRotY, fRotZ);
}

STREAMER_EXPORT int Streamer_OnPlayerSelectObject(int playerid, int type, int objectid, int modelid, float fX, float fY, float fZ)
{
	return core->getEvents()->OnPlayerSelectObject(playerid, type, objectid, modelid, fX, fY, fZ);
}

STREAMER_EXPORT int Streamer_OnPlayerPickUpPickup(int playerid, int pickupid)
{
	return core->getEvents()->OnPlayerPickUpPickup(playerid, pickupid);
}

STREAMER_EXPORT int Streamer_OnPlayerEnterCheckpoint(int playerid)
{
	return core->getEvents()->OnPlayerEnterCheckpoint(playerid);
}

STREAMER_EXPORT int Streamer_OnPlayerLeaveCheckpoint(int playerid)
{
	return core->getEvents()->OnPlayerLeaveCheckpoint(playerid);
}

STREAMER_EXPORT int Streamer_OnPlayerEnterRaceCheckpoint(int playerid)
{
	return core->getEvents()->OnPlayerEnterRaceCheckpoint(playerid);
}

STREAMER_EXPORT int Streamer_OnPlayerLeaveRaceCheckpoint(int playerid)
{
	return core->getEvents()->OnPlayerLeaveRaceCheckpoint(playerid);
}

STREAMER_EXPORT void Streamer_ProcessTick() {
	core->getStreamer()->startAutomaticUpdate();
}

#include <boost/function.hpp>

extern boost::function<void(int, int)> OnPlayerEnterDynamicArea;
extern boost::function<void(int, int)> OnPlayerLeaveDynamicArea;
extern boost::function<void(int)> OnDynamicObjectMoved;

STREAMER_EXPORT void SetOnPlayerEnterDynamicAreaCallback(STREAMER_TWOINTCALLBACK callback) {
	OnPlayerEnterDynamicArea = callback;
}

STREAMER_EXPORT void SetOnPlayerLeaveDynamicAreaCallback(STREAMER_TWOINTCALLBACK callback) {
	OnPlayerLeaveDynamicArea = callback;
}

STREAMER_EXPORT void SetOnDynamicObjectMovedCallback(STREAMER_ONEINTCALLBACK callback) {
	OnDynamicObjectMoved = callback;
}
