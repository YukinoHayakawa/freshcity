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

Grid::Grid()
{
	cellDistance = 360000.0f;
	cellSize = 300.0f;
	globalCell = SharedCell(new Cell());
	calculateTranslationMatrix();
}

void Grid::addArea(const Element::SharedArea &area)
{
	if (area->size > cellDistance)
	{
		globalCell->areas.insert(std::make_pair(area->areaID, area));
		area->cell.reset();
	}
	else
	{
		Eigen::Vector2f position = Eigen::Vector2f::Zero();
		switch (area->type)
		{
			case STREAMER_AREA_TYPE_CIRCLE:
			{
				if (area->attach)
				{
					position = Eigen::Vector2f(area->attach->position[0], area->attach->position[1]);
				}
				else
				{
					position = boost::get<Eigen::Vector2f>(area->position);
				}
			}
			break;
			case STREAMER_AREA_TYPE_RECTANGLE:
			{
				boost::geometry::centroid(boost::get<Element::Box2D>(area->position), position);
			}
			break;
			case STREAMER_AREA_TYPE_SPHERE:
			{
				if (area->attach)
				{
					position = Eigen::Vector2f(area->attach->position[0], area->attach->position[1]);
				}
				else
				{
					position = Eigen::Vector2f(boost::get<Eigen::Vector3f>(area->position)[0], boost::get<Eigen::Vector3f>(area->position)[1]);
				}
			}
			break;
			case STREAMER_AREA_TYPE_CUBE:
			{
				Eigen::Vector3f centroid = Eigen::Vector3f::Zero();
				boost::geometry::centroid(boost::get<Element::Box3D>(area->position), centroid);
				position = Eigen::Vector2f(centroid[0], centroid[1]);
			}
			break;
			case STREAMER_AREA_TYPE_POLYGON:
			{
				boost::geometry::centroid(boost::get<Element::Polygon2D>(area->position).get<0>(), position);
			}
			break;
		}
		CellID cellID = getCellID(position);
		cells[cellID]->areas.insert(std::make_pair(area->areaID, area));
		area->cell = cells[cellID];
	}
}

void Grid::addCheckpoint(const Element::SharedCheckpoint &checkpoint)
{
	if (checkpoint->streamDistance > cellDistance)
	{
		globalCell->checkpoints.insert(std::make_pair(checkpoint->checkpointID, checkpoint));
		checkpoint->cell.reset();
	}
	else
	{
		CellID cellID = getCellID(Eigen::Vector2f(checkpoint->position[0], checkpoint->position[1]));
		cells[cellID]->checkpoints.insert(std::make_pair(checkpoint->checkpointID, checkpoint));
		checkpoint->cell = cells[cellID];
	}
}

void Grid::addMapIcon(const Element::SharedMapIcon &mapIcon)
{
	if (mapIcon->streamDistance > cellDistance)
	{
		globalCell->mapIcons.insert(std::make_pair(mapIcon->mapIconID, mapIcon));
		mapIcon->cell.reset();
	}
	else
	{
		CellID cellID = getCellID(Eigen::Vector2f(mapIcon->position[0], mapIcon->position[1]));
		cells[cellID]->mapIcons.insert(std::make_pair(mapIcon->mapIconID, mapIcon));
		mapIcon->cell = cells[cellID];
	}
}

void Grid::addObject(const Element::SharedObject &object)
{
	if (object->streamDistance > cellDistance)
	{
		globalCell->objects.insert(std::make_pair(object->objectID, object));
		object->cell.reset();
	}
	else
	{
		Eigen::Vector2f position = Eigen::Vector2f::Zero();
		if (object->attach)
		{
			position = Eigen::Vector2f(object->attach->position[0], object->attach->position[1]);
		}
		else
		{
			position = Eigen::Vector2f(object->position[0], object->position[1]);
		}
		CellID cellID = getCellID(Eigen::Vector2f(position[0], position[1]));
		cells[cellID]->objects.insert(std::make_pair(object->objectID, object));
		object->cell = cells[cellID];
	}
}

void Grid::addPickup(const Element::SharedPickup &pickup)
{
	if (pickup->streamDistance > cellDistance)
	{
		globalCell->pickups.insert(std::make_pair(pickup->pickupID, pickup));
		pickup->cell.reset();
	}
	else
	{
		CellID cellID = getCellID(Eigen::Vector2f(pickup->position[0], pickup->position[1]));
		cells[cellID]->pickups.insert(std::make_pair(pickup->pickupID, pickup));
		pickup->cell = cells[cellID];
	}
}

void Grid::addRaceCheckpoint(const Element::SharedRaceCheckpoint &raceCheckpoint)
{
	if (raceCheckpoint->streamDistance > cellDistance)
	{
		globalCell->raceCheckpoints.insert(std::make_pair(raceCheckpoint->raceCheckpointID, raceCheckpoint));
		raceCheckpoint->cell.reset();
	}
	else
	{
		CellID cellID = getCellID(Eigen::Vector2f(raceCheckpoint->position[0], raceCheckpoint->position[1]));
		cells[cellID]->raceCheckpoints.insert(std::make_pair(raceCheckpoint->raceCheckpointID, raceCheckpoint));
		raceCheckpoint->cell = cells[cellID];
	}
}

void Grid::addTextLabel(const Element::SharedTextLabel &textLabel)
{
	if (textLabel->streamDistance > cellDistance)
	{
		globalCell->textLabels.insert(std::make_pair(textLabel->textLabelID, textLabel));
		textLabel->cell.reset();
	}
	else
	{
		Eigen::Vector2f position = Eigen::Vector2f::Zero();
		if (textLabel->attach)
		{
			position = Eigen::Vector2f(textLabel->attach->position[0], textLabel->attach->position[1]);
		}
		else
		{
			position = Eigen::Vector2f(textLabel->position[0], textLabel->position[1]);;
		}
		CellID cellID = getCellID(position);
		cells[cellID]->textLabels.insert(std::make_pair(textLabel->textLabelID, textLabel));
		textLabel->cell = cells[cellID];
	}
}

void Grid::calculateTranslationMatrix()
{
	translationMatrix << 0.0f, 0.0f, cellSize, cellSize, cellSize * -1.0f, 0.0f, cellSize * -1.0f, cellSize, cellSize * -1.0f,
	                     0.0f, cellSize, 0.0f, cellSize, 0.0f, cellSize * -1.0f, cellSize, cellSize * -1.0f, cellSize * -1.0f;
}

void Grid::cleanVisibleCell(SharedCell &visibleCell, const boost::unordered_set<CellID> &discoveredCells)
{
	if (!visibleCell->areas.empty())
	{
		boost::unordered_map<int, Element::SharedArea>::iterator a = visibleCell->areas.begin();
		while (a != visibleCell->areas.end())
		{
			boost::unordered_set<CellID>::iterator d = discoveredCells.find(a->second->cell->id);
			if (d != discoveredCells.end())
			{
				a = visibleCell->areas.erase(a);
			}
			else
			{
				++a;
			}
		}
	}
	if (!visibleCell->checkpoints.empty())
	{
		boost::unordered_map<int, Element::SharedCheckpoint>::iterator c = visibleCell->checkpoints.begin();
		while (c != visibleCell->checkpoints.end())
		{
			boost::unordered_set<CellID>::iterator d = discoveredCells.find(c->second->cell->id);
			if (d != discoveredCells.end())
			{
				c = visibleCell->checkpoints.erase(c);
			}
			else
			{
				++c;
			}
		}
	}
	if (!visibleCell->mapIcons.empty())
	{
		boost::unordered_map<int, Element::SharedMapIcon>::iterator m = visibleCell->mapIcons.begin();
		while (m != visibleCell->mapIcons.end())
		{
			boost::unordered_set<CellID>::iterator d = discoveredCells.find(m->second->cell->id);
			if (d != discoveredCells.end())
			{
				m = visibleCell->mapIcons.erase(m);
			}
			else
			{
				++m;
			}
		}
	}
	if (!visibleCell->objects.empty())
	{
		boost::unordered_map<int, Element::SharedObject>::iterator o = visibleCell->objects.begin();
		while (o != visibleCell->objects.end())
		{
			boost::unordered_set<CellID>::iterator d = discoveredCells.find(o->second->cell->id);
			if (d != discoveredCells.end())
			{
				o = visibleCell->objects.erase(o);
			}
			else
			{
				++o;
			}
		}
	}
	if (!visibleCell->raceCheckpoints.empty())
	{
		boost::unordered_map<int, Element::SharedRaceCheckpoint>::iterator t = visibleCell->raceCheckpoints.begin();
		while (t != visibleCell->raceCheckpoints.end())
		{
			boost::unordered_set<CellID>::iterator d = discoveredCells.find(t->second->cell->id);
			if (d != discoveredCells.end())
			{
				t = visibleCell->raceCheckpoints.erase(t);
			}
			else
			{
				++t;
			}
		}
	}
	if (!visibleCell->pickups.empty())
	{
		boost::unordered_map<int, Element::SharedPickup>::iterator p = visibleCell->pickups.begin();
		while (p != visibleCell->pickups.end())
		{
			boost::unordered_set<CellID>::iterator d = discoveredCells.find(p->second->cell->id);
			if (d != discoveredCells.end())
			{
				p = visibleCell->pickups.erase(p);
			}
			else
			{
				++p;
			}
		}
	}
	if (!visibleCell->textLabels.empty())
	{
		boost::unordered_map<int, Element::SharedTextLabel>::iterator t = visibleCell->textLabels.begin();
		while (t != visibleCell->textLabels.end())
		{
			boost::unordered_set<CellID>::iterator d = discoveredCells.find(t->second->cell->id);
			if (d != discoveredCells.end())
			{
				t = visibleCell->textLabels.erase(t);
			}
			else
			{
				++t;
			}
		}
	}
}

void Grid::eraseAllItems(int type)
{
	boost::unordered_map<CellID, SharedCell> temporaryCells(cells);
	switch (type)
	{
		case STREAMER_TYPE_OBJECT:
		{
			globalCell->objects.clear();
			for (boost::unordered_map<CellID, SharedCell>::iterator c = temporaryCells.begin(); c != temporaryCells.end(); ++c)
			{
				c->second->objects.clear();
				eraseCellIfEmpty(c->second);
			}
		}
		break;
		case STREAMER_TYPE_PICKUP:
		{
			globalCell->pickups.clear();
			for (boost::unordered_map<CellID, SharedCell>::iterator c = temporaryCells.begin(); c != temporaryCells.end(); ++c)
			{
				c->second->pickups.clear();
				eraseCellIfEmpty(c->second);
			}
		}
		break;
		case STREAMER_TYPE_CP:
		{
			globalCell->checkpoints.clear();
			for (boost::unordered_map<CellID, SharedCell>::iterator c = temporaryCells.begin(); c != temporaryCells.end(); ++c)
			{
				c->second->checkpoints.clear();
				eraseCellIfEmpty(c->second);
			}
		}
		break;
		case STREAMER_TYPE_RACE_CP:
		{
			globalCell->raceCheckpoints.clear();
			for (boost::unordered_map<CellID, SharedCell>::iterator c = temporaryCells.begin(); c != temporaryCells.end(); ++c)
			{
				c->second->raceCheckpoints.clear();
				eraseCellIfEmpty(c->second);
			}
		}
		break;
		case STREAMER_TYPE_MAP_ICON:
		{
			globalCell->mapIcons.clear();
			for (boost::unordered_map<CellID, SharedCell>::iterator c = temporaryCells.begin(); c != temporaryCells.end(); ++c)
			{
				c->second->mapIcons.clear();
				eraseCellIfEmpty(c->second);
			}
		}
		break;
		case STREAMER_TYPE_3D_TEXT_LABEL:
		{
			globalCell->textLabels.clear();
			for (boost::unordered_map<CellID, SharedCell>::iterator c = temporaryCells.begin(); c != temporaryCells.end(); ++c)
			{
				c->second->textLabels.clear();
				eraseCellIfEmpty(c->second);
			}
		}
		break;
		case STREAMER_TYPE_AREA:
		{
			globalCell->areas.clear();
			for (boost::unordered_map<CellID, SharedCell>::iterator c = temporaryCells.begin(); c != temporaryCells.end(); ++c)
			{
				c->second->areas.clear();
				eraseCellIfEmpty(c->second);
			}
		}
		break;
	}
}

void Grid::eraseCellIfEmpty(const SharedCell &cell)
{
	if (cell->areas.empty() && cell->checkpoints.empty() && cell->mapIcons.empty() && cell->objects.empty() && cell->pickups.empty() && cell->raceCheckpoints.empty() && cell->textLabels.empty())
	{
		cells.erase(cell->id);
	}
}

void Grid::findAllCells(StreamerPlayer &player, std::vector<SharedCell> &playerCells)
{
	boost::unordered_set<CellID> discoveredCells;
	for (int i = 0; i < translationMatrix.cols(); ++i)
	{
		Eigen::Vector2f position = Eigen::Vector2f(player.position[0], player.position[1]) + translationMatrix.col(i);
		boost::unordered_map<CellID, SharedCell>::iterator c = cells.find(getCellID(position, false));
		if (c != cells.end())
		{
			discoveredCells.insert(c->first);
			playerCells.push_back(c->second);
		}
	}
	cleanVisibleCell(player.visibleCell, discoveredCells);
	playerCells.push_back(globalCell);
	playerCells.push_back(player.visibleCell);
	player.visibleCell = SharedCell(new Cell());
}

void Grid::findMinimalCells(StreamerPlayer &player, std::vector<SharedCell> &playerCells)
{
	for (int i = 0; i < translationMatrix.cols(); ++i)
	{
		Eigen::Vector2f position = Eigen::Vector2f(player.position[0], player.position[1]) + translationMatrix.col(i);
		boost::unordered_map<CellID, SharedCell>::iterator c = cells.find(getCellID(position, false));
		if (c != cells.end())
		{
			playerCells.push_back(c->second);
		}
	}
	playerCells.push_back(globalCell);
}

CellID Grid::getCellID(const Eigen::Vector2f &position, bool insert)
{
	Element::Box2D box;
	box.min_corner()[0] = floor((position[0] / cellSize)) * cellSize;
	box.max_corner()[0] = box.min_corner()[0] + cellSize;
	box.min_corner()[1] = floor((position[1] / cellSize)) * cellSize;
	box.max_corner()[1] = box.min_corner()[1] + cellSize;
	Eigen::Vector2f centroid = Eigen::Vector2f::Zero();
	boost::geometry::centroid(box, centroid);
	CellID cellID = std::make_pair(static_cast<int>(centroid[0]), static_cast<int>(centroid[1]));
	if (insert)
	{
		boost::unordered_map<CellID, SharedCell>::iterator c = cells.find(cellID);
		if (c == cells.end())
		{
			SharedCell cell(new Cell(cellID));
			cells[cellID] = cell;
		}
	}
	return cellID;
}

void Grid::rebuildGrid()
{
	cells.clear();
	globalCell = SharedCell(new Cell());
	calculateTranslationMatrix();
	for (boost::unordered_map<int, Element::SharedArea>::iterator a = core->getData()->areas.begin(); a != core->getData()->areas.end(); ++a)
	{
		addArea(a->second);
	}
	for (boost::unordered_map<int, Element::SharedCheckpoint>::iterator c = core->getData()->checkpoints.begin(); c != core->getData()->checkpoints.end(); ++c)
	{
		addCheckpoint(c->second);
	}
	for (boost::unordered_map<int, Element::SharedMapIcon>::iterator m = core->getData()->mapIcons.begin(); m != core->getData()->mapIcons.end(); ++m)
	{
		addMapIcon(m->second);
	}
	for (boost::unordered_map<int, Element::SharedObject>::iterator o = core->getData()->objects.begin(); o != core->getData()->objects.end(); ++o)
	{
		addObject(o->second);
	}
	for (boost::unordered_map<int, Element::SharedPickup>::iterator p = core->getData()->pickups.begin(); p != core->getData()->pickups.end(); ++p)
	{
		addPickup(p->second);
	}
	for (boost::unordered_map<int, Element::SharedRaceCheckpoint>::iterator r = core->getData()->raceCheckpoints.begin(); r != core->getData()->raceCheckpoints.end(); ++r)
	{
		addRaceCheckpoint(r->second);
	}
	for (boost::unordered_map<int, Element::SharedTextLabel>::iterator t = core->getData()->textLabels.begin(); t != core->getData()->textLabels.end(); ++t)
	{
		addTextLabel(t->second);
	}
}

void Grid::removeArea(const Element::SharedArea &area, bool reassign)
{
	bool found = false;
	if (area->cell)
	{
		boost::unordered_map<CellID, SharedCell>::iterator c = cells.find(area->cell->id);
		if (c != cells.end())
		{
			boost::unordered_map<int, Element::SharedArea>::iterator a = c->second->areas.find(area->areaID);
			if (a != c->second->areas.end())
			{
				c->second->areas.quick_erase(a);
				eraseCellIfEmpty(c->second);
				found = true;
			}
		}
	}
	else
	{
		boost::unordered_map<int, Element::SharedArea>::iterator a = globalCell->areas.find(area->areaID);
		if (a != globalCell->areas.end())
		{
			globalCell->areas.quick_erase(a);
			found = true;
		}
	}
	if (found)
	{
		if (reassign)
		{
			addArea(area);
		}
		else
		{
			if (area->attach)
			{
				core->getStreamer()->attachedAreas.erase(area);
			}
		}
	}
}

void Grid::removeCheckpoint(const Element::SharedCheckpoint &checkpoint, bool reassign)
{
	bool found = false;
	if (checkpoint->cell)
	{
		boost::unordered_map<CellID, SharedCell>::iterator c = cells.find(checkpoint->cell->id);
		if (c != cells.end())
		{
			boost::unordered_map<int, Element::SharedCheckpoint>::iterator d = c->second->checkpoints.find(checkpoint->checkpointID);
			if (d != c->second->checkpoints.end())
			{
				c->second->checkpoints.quick_erase(d);
				eraseCellIfEmpty(c->second);
				found = true;
			}
		}
	}
	else
	{
		boost::unordered_map<int, Element::SharedCheckpoint>::iterator c = globalCell->checkpoints.find(checkpoint->checkpointID);
		if (c != globalCell->checkpoints.end())
		{
			globalCell->checkpoints.quick_erase(c);
			found = true;
		}
	}
	if (found)
	{
		if (reassign)
		{
			addCheckpoint(checkpoint);
		}
	}
}

void Grid::removeMapIcon(const Element::SharedMapIcon &mapIcon, bool reassign)
{
	bool found = false;
	if (mapIcon->cell)
	{
		boost::unordered_map<CellID, SharedCell>::iterator c = cells.find(mapIcon->cell->id);
		if (c != cells.end())
		{
			boost::unordered_map<int, Element::SharedMapIcon>::iterator m = c->second->mapIcons.find(mapIcon->mapIconID);
			if (m != c->second->mapIcons.end())
			{
				c->second->mapIcons.quick_erase(m);
				eraseCellIfEmpty(c->second);
				found = true;
			}
		}
	}
	else
	{
		boost::unordered_map<int, Element::SharedMapIcon>::iterator m = globalCell->mapIcons.find(mapIcon->mapIconID);
		if (m != globalCell->mapIcons.end())
		{
			globalCell->mapIcons.quick_erase(m);
			found = true;
		}
	}
	if (found)
	{
		if (reassign)
		{
			addMapIcon(mapIcon);
		}
	}
}

void Grid::removeObject(const Element::SharedObject &object, bool reassign)
{
	bool found = false;
	if (object->cell)
	{
		boost::unordered_map<CellID, SharedCell>::iterator c = cells.find(object->cell->id);
		if (c != cells.end())
		{
			boost::unordered_map<int, Element::SharedObject>::iterator o = c->second->objects.find(object->objectID);
			if (o != c->second->objects.end())
			{
				c->second->objects.quick_erase(o);
				eraseCellIfEmpty(c->second);
				found = true;
			}
		}
	}
	else
	{
		boost::unordered_map<int, Element::SharedObject>::iterator o = globalCell->objects.find(object->objectID);
		if (o != globalCell->objects.end())
		{
			globalCell->objects.quick_erase(o);
			found = true;
		}
	}
	if (found)
	{
		if (reassign)
		{
			addObject(object);
		}
		else
		{
			if (object->attach)
			{
				core->getStreamer()->attachedObjects.erase(object);
			}
			if (object->move)
			{
				core->getStreamer()->movingObjects.erase(object);
			}
		}
	}
}

void Grid::removePickup(const Element::SharedPickup &pickup, bool reassign)
{
	bool found = false;
	if (pickup->cell)
	{
		boost::unordered_map<CellID, SharedCell>::iterator c = cells.find(pickup->cell->id);
		if (c != cells.end())
		{
			boost::unordered_map<int, Element::SharedPickup>::iterator p = c->second->pickups.find(pickup->pickupID);
			if (p != c->second->pickups.end())
			{
				c->second->pickups.quick_erase(p);
				eraseCellIfEmpty(c->second);
				found = true;
			}
		}
	}
	else
	{
		boost::unordered_map<int, Element::SharedPickup>::iterator p = globalCell->pickups.find(pickup->pickupID);
		if (p != globalCell->pickups.end())
		{
			globalCell->pickups.quick_erase(p);
			found = true;
		}
	}
	if (found)
	{
		if (reassign)
		{
			addPickup(pickup);
		}
	}
}

void Grid::removeRaceCheckpoint(const Element::SharedRaceCheckpoint &raceCheckpoint, bool reassign)
{
	bool found = false;
	if (raceCheckpoint->cell)
	{
		boost::unordered_map<CellID, SharedCell>::iterator c = cells.find(raceCheckpoint->cell->id);
		if (c != cells.end())
		{
			boost::unordered_map<int, Element::SharedRaceCheckpoint>::iterator r = c->second->raceCheckpoints.find(raceCheckpoint->raceCheckpointID);
			if (r != c->second->raceCheckpoints.end())
			{
				c->second->raceCheckpoints.quick_erase(r);
				eraseCellIfEmpty(c->second);
				found = true;
			}
		}
	}
	else
	{
		boost::unordered_map<int, Element::SharedRaceCheckpoint>::iterator r = globalCell->raceCheckpoints.find(raceCheckpoint->raceCheckpointID);
		if (r != globalCell->raceCheckpoints.end())
		{
			globalCell->raceCheckpoints.quick_erase(r);
			found = true;
		}
	}
	if (found)
	{
		if (reassign)
		{
			addRaceCheckpoint(raceCheckpoint);
		}
	}
}

void Grid::removeTextLabel(const Element::SharedTextLabel &textLabel, bool reassign)
{
	bool found = false;
	if (textLabel->cell)
	{
		boost::unordered_map<CellID, SharedCell>::iterator c = cells.find(textLabel->cell->id);
		if (c != cells.end())
		{
			boost::unordered_map<int, Element::SharedTextLabel>::iterator t = c->second->textLabels.find(textLabel->textLabelID);
			if (t != c->second->textLabels.end())
			{
				c->second->textLabels.quick_erase(t);
				eraseCellIfEmpty(c->second);
				found = true;
			}
		}
	}
	else
	{
		boost::unordered_map<int, Element::SharedTextLabel>::iterator t = globalCell->textLabels.find(textLabel->textLabelID);
		if (t != globalCell->textLabels.end())
		{
			globalCell->textLabels.quick_erase(t);
			found = true;
		}
	}
	if (found)
	{
		if (reassign)
		{
			addTextLabel(textLabel);
		}
		else
		{
			if (textLabel->attach)
			{
				core->getStreamer()->attachedTextLabels.erase(textLabel);
			}
		}
	}
}
